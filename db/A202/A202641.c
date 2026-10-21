/*
 * A202641  Number of (n+2)X4 binary arrays avoiding patterns 000 and 111 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 15, u64 n <= 33, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over rows; no explicit recurrence formula used. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Check if a 4‑bit row contains a horizontal run 000 or 111. */
static inline bool row_is_valid(uint8_t r) {
    /* Two triples: bits 0‑1‑2 and 1‑2‑3. */
    uint8_t t0 = (r >> 0) & 0x7;   /* bits 0‑1‑2 */
    uint8_t t1 = (r >> 1) & 0x7;   /* bits 1‑2‑3 */
    return !(t0 == 0 || t0 == 7 || t1 == 0 || t1 == 7);
}

seq_t a(idx_t n) {
    /* Total number of rows in the array.               */
    const idx_t rows = n + 2;          /* (n+2) × 4 array */

    /* Validity of all 16 possible 4‑bit rows. */
    static bool valid[16];
    static bool init = false;
    if (!init) {
        for (uint8_t r = 0; r < 16; ++r)
            valid[r] = row_is_valid(r);
        init = true;
    }

    /* Count of rows that are valid horizontally. */
    uint8_t valcnt = 0;
    for (uint8_t r = 0; r < 16; ++r)
        if (valid[r]) ++valcnt;

    /* Special case: only one row. */
    if (rows == 1)
        return (seq_t)valcnt;

    /* DP state: (prev2 << 4) | prev1, each 4 bits. */
    seq_t dp1[256] = {0};
    seq_t dp2[256] = {0};
    seq_t *cur = dp1;
    seq_t *nxt = dp2;

    /* Initialize for the first two rows. */
    for (uint8_t r1 = 0; r1 < 16; ++r1) {
        if (!valid[r1]) continue;
        for (uint8_t r2 = 0; r2 < 16; ++r2) {
            if (!valid[r2]) continue;
            cur[(r1 << 4) | r2] = (seq_t)1;
        }
    }
    if (rows == 2) {
        seq_t sum = 0;
        for (int i = 0; i < 256; ++i)
            sum += cur[i];
        return sum;
    }

    /* Process rows 3..rows. */
    for (idx_t r = 3; r <= rows; ++r) {
        memset(nxt, 0, sizeof(dp1));
        for (int state = 0; state < 256; ++state) {
            seq_t ways = cur[state];
            if (!ways) continue;
            uint8_t prev2 = (state >> 4) & 0xF;
            uint8_t prev1 = state & 0xF;
            for (uint8_t currow = 0; currow < 16; ++currow) {
                if (!valid[currow]) continue;

                bool ok = true;

                /* Vertical 3‑run check in each column. */
                for (int col = 0; col < 4; ++col) {
                    int sum = ((prev2 >> col) & 1) +
                              ((prev1 >> col) & 1) +
                              ((currow >> col) & 1);
                    if (sum == 0 || sum == 3) { ok = false; break; }
                }
                if (!ok) continue;

                /* NW‑SE diagonal 3‑run check (only columns 3 and 4). */
                for (int col = 2; col < 4; ++col) {
                    int sum = ((prev2 >> (col - 2)) & 1) +
                              ((prev1 >> (col - 1)) & 1) +
                              ((currow >> col) & 1);
                    if (sum == 0 || sum == 3) { ok = false; break; }
                }
                if (!ok) continue;

                /* Transition to new state. */
                int new_state = (prev1 << 4) | currow;
                nxt[new_state] += ways;
            }
        }
        /* Prepare for next iteration. */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum all terminal states. */
    seq_t ans = 0;
    for (int i = 0; i < 256; ++i)
        ans += cur[i];
    return ans;
}
