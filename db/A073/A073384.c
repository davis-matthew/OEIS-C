/*
 * A073384  Seventh convolution of A000129(n+1) (generalized (2,1)-Fibonacci,
 *          called Pell numbers), n>=0, with itself.
 * https://oeis.org/A073384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} 2^(n-2k) * binomial(n-k+7, 7) *
 *            binomial(n-k, k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 32, u128 n <= 76,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the utility functions pow2 and binomial. */
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k)
    {
        seq_t term = pow2(n - 2 * k);                     /* 2^(n-2k) */
        term *= binomial(n - k + 7, 7);                   /* binomial(n-k+7, 7) */
        term *= binomial(n - k, k);                      /* binomial(n-k, k) */
        sum += term;
    }
    return sum;
}
