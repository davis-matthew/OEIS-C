/*
 * A240272  Number of 2Xn 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or one plus the sum of the elements
 *          above it or one plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A240272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 2×n arrays with entries 0…3 satisfying the OEIS
 *            A240272 constraint. Computed by dynamic programming over 64
 *            states (sums modulo 4)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>


seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but safe */

    const int STATES = 64;
    seq_t dp_cur[STATES];
    seq_t dp_next[STATES];

    /* initial state: sums=0, previous top=0 */
    for (int i = 0; i < STATES; ++i) dp_cur[i] = 0;
    dp_cur[0] = 1;

    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < STATES; ++i) dp_next[i] = 0;

        for (int s = 0; s < STATES; ++s) {
            seq_t ways = dp_cur[s];
            if (ways == 0) continue;

            int sum_top    = s >> 4;          /* s/16   */
            int rem        = s & 15;          /* s%16   */
            int sum_bottom = rem >> 2;        /* rem/4  */
            int prev_t     = rem & 3;         /* rem%4  */

            for (int t = 0; t < 4; ++t) {
                if (t == sum_top) continue;          /* forbid left sum */
                if (t == 1) continue;                /* forbid (0+1)%4 above/diag */

                int inc_t = (t + 1) & 3;              /* (t+1)%4 */

                for (int b = 0; b < 4; ++b) {
                    if (b == sum_bottom) continue;   /* forbid left bottom sum */
                    if (b == inc_t) continue;        /* forbid (t+1)%4 above bottom */
                    if (b == ((prev_t + 1) & 3)) continue; /* forbid (prev_t+1)%4 diag */

                    int new_sum_top    = (sum_top + t) & 3;
                    int new_sum_bottom = (sum_bottom + b) & 3;
                    int new_idx = (new_sum_top << 4) | (new_sum_bottom << 2) | t;

                    dp_next[new_idx] += ways;
                }
            }
        }

        /* copy next into current for next iteration */
        for (int i = 0; i < STATES; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum all ending states */
    seq_t total = 0;
    for (int i = 0; i < STATES; ++i) {
        total += dp_cur[i];
    }
    return total;
}
