/*
 * A230783  Number of nX3 0..3 arrays x(i,j) with each element horizontally,
 *          vertically, diagonally or antidiagonally next to at least one
 *          element with value (x(i,j)+1) mod 4, and upper left element zero.
 * https://oeis.org/A230783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 arrays of values 0..3, with upper‑left
 *            element 0, such that each cell has at least one neighbor
 *            (horizontally, vertically, diagonally or antidiagonally) whose
 *            value is (x+1) mod 4. Computed by a DP over rows: for each row
 *            we keep the 3‑bit mask of cells that still need a belo...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* idx_t and seq_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* The sequence is undefined below n=1, but we
       simply return 0 for n==0. */
    if (n == 0) return 0;

    /* Transition tables: satMask[p][q] tells which cells
       in pattern p are satisfied by pattern q placed below.
       newMask[p][q] gives the mask of cells in q that still
       need a neighbour below after adding q below p. */
    static uint8_t satMask[64][64];
    static uint8_t newMask[64][64];
    static bool init_tbl = false;

    if (!init_tbl) {
        for (uint8_t p = 0; p < 64; ++p) {
            for (uint8_t q = 0; q < 64; ++q) {
                /* cells of p satisfied by q */
                uint8_t sat = 0;
                for (int j = 0; j < 3; ++j) {
                    uint8_t valP = (p >> (2 * j)) & 3;
                    uint8_t target = (valP + 1) & 3;
                    bool ok = false;
                    for (int k = j - 1; k <= j + 1; ++k) {
                        if (k < 0 || k > 2) continue;
                        uint8_t valQ = (q >> (2 * k)) & 3;
                        if (valQ == target) { ok = true; break; }
                    }
                    if (ok) sat |= (1 << j);
                }
                satMask[p][q] = sat;

                /* mask of q that still needs a neighbour below */
                uint8_t nm = 0;
                for (int j = 0; j < 3; ++j) {
                    uint8_t valQ = (q >> (2 * j)) & 3;
                    uint8_t target = (valQ + 1) & 3;
                    bool have = false;
                    /* neighbours within q (left/right) */
                    if (j > 0 && ((q >> (2 * (j - 1))) & 3) == target) have = true;
                    if (j < 2 && ((q >> (2 * (j + 1))) & 3) == target) have = true;
                    /* neighbours from row p (up-left, up, up-right) */
                    if (j > 0 && ((p >> (2 * (j - 1))) & 3) == target) have = true;
                    if (((p >> (2 * j)) & 3) == target) have = true;
                    if (j < 2 && ((p >> (2 * (j + 1))) & 3) == target) have = true;
                    if (!have) nm |= (1 << j);
                }
                newMask[p][q] = nm;
            }
        }
        init_tbl = true;
    }

    /* DP tables: current and next row states.
       dp[pattern][mask] = number of ways. */
    static seq_t dp[64][8];
    static seq_t dp_next[64][8];

    /* Initialise dp for the first row. */
    for (int i = 0; i < 64; ++i)
        for (int m = 0; m < 8; ++m)
            dp[i][m] = 0;

    for (uint8_t p = 0; p < 64; ++p) {
        /* upper‑left element must be 0 */
        if ((p & 3) != 0) continue;
        /* compute mask for first row (needs from below) */
        uint8_t m = 0;
        for (int j = 0; j < 3; ++j) {
            uint8_t valP = (p >> (2 * j)) & 3;
            uint8_t target = (valP + 1) & 3;
            bool have = false;
            if (j > 0 && ((p >> (2 * (j - 1))) & 3) == target) have = true;
            if (j < 2 && ((p >> (2 * (j + 1))) & 3) == target) have = true;
            if (!have) m |= (1 << j);
        }
        dp[p][m] = 1;
    }

    /* Add rows 2..n */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < 64; ++i)
            for (int m = 0; m < 8; ++m)
                dp_next[i][m] = 0;

        for (uint8_t p = 0; p < 64; ++p) {
            for (int need = 0; need < 8; ++need) {
                seq_t cur = dp[p][need];
                if (!cur) continue;
                for (uint8_t q = 0; q < 64; ++q) {
                    /* cells that need a neighbour below must be satisfied by q */
                    if ((need & ~satMask[p][q]) != 0) continue;
                    uint8_t nm = newMask[p][q];
                    dp_next[q][nm] += cur;
                }
            }
        }
        /* swap dp and dp_next */
        for (int i = 0; i < 64; ++i)
            for (int m = 0; m < 8; ++m)
                dp[i][m] = dp_next[i][m];
    }

    /* Sum all states of the last row with mask 0 (all cells satisfied) */
    seq_t ans = 0;
    for (uint8_t p = 0; p < 64; ++p)
        ans += dp[p][0];

    return ans;
}
