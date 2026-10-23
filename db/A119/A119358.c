/*
 * A119358  Number of n-element subsets of [2n] having an even sum.
 * https://oeis.org/A119358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} C(n,2k)^2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t ans   = 0;      /* sum of squares */
    seq_t binom = 1;      /* C(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        /* add square of binomial if k is even */
        if ((k & 1) == 0) {
            ans += binom * binom;
        }

        /* stop after computing the last binomial */
        if (k == n) {
            break;
        }

        /* next binomial: C(n, k+1) = C(n,k) * (n-k)/(k+1)  */
        binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
    }

    return ans;
}
