/*
 * A047161  Number of nonempty subsets of {1,2,...,n} in which exactly 1/3 of
 *          the elements are <= n/2.
 * https://oeis.org/A047161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1} binomial(floor(n/2),k) *
 *            binomial(ceil(n/2),2*k)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 38, u64 n <= 72, u128 n <= 142,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"        /* provides seq_t binom(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* All terms for n < 2 are 0 by definition. */
    if (n <= 1u)
        return 0;

    idx_t L = n / 2;          /* floor(n/2)   */
    idx_t R = n - L;          /* ceil(n/2)    */

    seq_t sum = 0;
    for (idx_t k = 1; k * 2 <= R; ++k) {
        seq_t left  = binom(L, k);        /* C(L,k)   */
        seq_t right = binom(R, 2 * k);    /* C(R,2k)  */
        sum += left * right;
    }
    return sum;
}
