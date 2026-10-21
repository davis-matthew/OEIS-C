/*
 * A073382  Fifth convolution of A000129(n+1) (generalized (2,1)-Fibonacci,
 *          called Pell numbers), n>=0, with itself.
 * https://oeis.org/A073382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} 2^(n-2k) * binomial(n-k+5,5) *
 *            binomial(n-k,k)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 35, u128 n <= 81,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   /* binom(idx_t, idx_t) -> seq_t */
#include "../utils/pow2.h"    /* pow2(idx_t) -> seq_t */

seq_t a(idx_t n)
{
    idx_t maxk = n / 2;          /* floor(n/2) */
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term = binom(n - k + 5, 5);           /* binomial(n-k+5,5) */
        term *= binom(n - k, k);                   /* * binomial(n-k,k)    */
        term *= pow2(n - 2 * k);                   /* * 2^(n-2k)           */
        sum += term;
    }
    return sum;
}
