/*
 * A188495  Number of permutations p on the set [n] with the properties that
 *          abs(p(i)-i) <= 3 for all i, p(1) <= 2, and p(4) >= 2.
 * https://oeis.org/A188495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the permanent of the n×n 0–1 matrix that has ones on its
 *            diagonal, on its three superdiagonals and on its three
 *            subdiagonals, except for the zeros at (1,4), (3,1) and (4,1).
 *            Implemented by dynamic programming over columns: at each step
 *            the state is a 7‑bit mask describing which rows in th...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* DP tables for 128 possible masks (7 bits). */
    seq_t dp_cur[128] = {0};
    seq_t dp_next[128] = {0};

    /* initial mask for column 1 (rows 1..4 that exist). */
    uint8_t init_mask = 0;
    for (int k = 0; k < 7; ++k) {
        int r = k - 2;           /* row = 1-3 + k = k-2 */
        if (r >= 1 && (idx_t)r <= n)
            init_mask |= (uint8_t)(1u << k);
    }
    dp_cur[init_mask] = 1;

    seq_t ans = 0;

    for (idx_t j = 1; j <= n; ++j) {
        /* clear next table */
        for (int t = 0; t < 128; ++t) dp_next[t] = 0;

        for (int m = 0; m < 128; ++m) {
            if (dp_cur[m] == 0) continue;

            for (int k = 0; k < 7; ++k) {
                if (!(m & (1u << k))) continue;            /* row not free */

                int r = (int)j - 3 + k;     /* candidate row */
                if (r < 1 || (idx_t)r > n) continue;

                /* constraints */
                if (j == 1 && r > 2) continue;    /* p(1) ≤ 2 */
                if (j == 4 && r == 1) continue;   /* p(4) ≥ 2 */

                /* compute new mask for column j+1 */
                uint8_t new_mask = (uint8_t)(m >> 1);          /* drop leftmost row */
                if (k > 0) {
                    uint8_t k_new = (uint8_t)(k - 1);
                    new_mask &= ~(uint8_t)(1u << k_new);      /* clear chosen row */
                }
                if (j + 4 <= n) {
                    new_mask |= (uint8_t)(1u << 6);           /* add new rightmost row */
                }

                if (j == n) {
                    if (new_mask == 0) ans += dp_cur[m];
                } else {
                    dp_next[new_mask] += dp_cur[m];
                }
            }
        }
        /* move to next column */
        for (int t = 0; t < 128; ++t) dp_cur[t] = dp_next[t];
    }

    return ans;
}
