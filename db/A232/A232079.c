/*
 * A232079  Number of (3+1)X(n+1) 0..1 arrays with every element equal to some
 *          horizontal, diagonal or antidiagonal neighbor, with top left
 *          element zero.
 * https://oeis.org/A232079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is computed by a transfer–matrix DP for the number of
 *           4×(n+1) binary arrays (rows indexed 0‑3, columns 0‑n) with
 *           the property that each entry equals at least one of its
 *           horizontal, diagonal or antidiagonal neighbors, and with
 *           the top‑left entry fixed to 0.  The DP keeps for each
 *           column a 4‑bit pattern of the column and a 4‑bit mask of
 *           entries that still need a matching neighbor in the next
 *           column.  The final column must have mask 0.
 *
 * The implementation works for all builds: seq_t can be any unsigned
 * integer type (including a big‑integer class).  The algorithm is
 * O(n) time with a 256‑state transfer matrix, easily fast enough for
 * n up to 10000.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* precompute cross‑satisfaction masks: cross_ok[L][R] has bit r set
       iff cell r in column R is satisfied by some same‑valued neighbor
       in column L (row r, r‑1, or r+1). */
    static uint8_t cross_ok[16][16];
    static int initialized = 0;
    if (!initialized) {
        initialized = 1;
        for (int L = 0; L < 16; ++L) {
            for (int R = 0; R < 16; ++R) {
                uint8_t mask = 0;
                for (int r = 0; r < 4; ++r) {
                    int valR = (R >> r) & 1;
                    int matches = 0;
                    /* same row */
                    if ((((L >> r) & 1) == valR)) matches = 1;
                    /* up‑right (row r‑1) */
                    if (!matches && r > 0 && (((L >> (r - 1)) & 1) == valR)) matches = 1;
                    /* down‑right (row r+1) */
                    if (!matches && r < 3 && (((L >> (r + 1)) & 1) == valR)) matches = 1;
                    if (matches) mask |= 1 << r;
                }
                cross_ok[L][R] = mask;
            }
        }
    }

    /* DP over 256 states: index = pattern << 4 | mask (both 4‑bit). */
    static seq_t dp[256], next[256];
    const uint8_t ALL = 0xF;        /* 1111b : all rows unsatisfied */

    /* initialise dp for column 0 (top‑left entry fixed to 0). */
    for (int i = 0; i < 256; ++i) dp[i] = 0;
    for (int p = 0; p < 16; ++p) {
        if ((p & 1) == 0) {          /* top‑left (row 0) must be 0 */
            dp[(p << 4) | ALL] = 1;
        }
    }

    /* perform n transitions: from column 0 to column n (total n+1 columns). */
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < 256; ++i) next[i] = 0;
        for (int p = 0; p < 16; ++p) {
            for (int m = 0; m < 16; ++m) {
                seq_t cur = dp[(p << 4) | m];
                if (cur == 0) continue;
                /* iterate over all possible next‑column patterns q   */
                for (int q = 0; q < 16; ++q) {
                    /* condition: every unsatisfied cell in p must be matched by q */
                    uint8_t need = m;
                    if ((need & (~cross_ok[q][p])) != 0) continue;
                    /* compute new mask for column q: rows still needing a right neighbor */
                    uint8_t nmask = (~cross_ok[p][q]) & ALL;
                    next[(q << 4) | nmask] += cur;
                }
            }
        }
        /* swap dp and next */
        for (int i = 0; i < 256; ++i) dp[i] = next[i];
    }

    /* sum counts of final configurations whose last column has no unsatisfied cell. */
    seq_t result = 0;
    for (int p = 0; p < 16; ++p) {
        result += dp[(p << 4) | 0];
    }
    return result;
}
