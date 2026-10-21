/*
 * A073383  Sixth convolution of A000129(n+1) (generalized (2,1)-Fibonacci,
 *          called Pell numbers), n>=0, with itself.
 * https://oeis.org/A073383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} 2^(n-2k) * binomial(n-k+6, 6) *
 *            binomial(n-k, k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 34, u128 n <= 79,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* k runs from 0 to ⌊n/2⌋ */
    const idx_t max_k = n >> 1;          /* floor division by 2 */
    for (idx_t k = 0; k <= max_k; ++k)
    {
        /* compute 2^(n-2k) */
        seq_t term = pow2(n - (k << 1));

        /* multiply by binomial(n-k+6, 6) */
        term *= binomial(n - k + 6, 6);

        /* multiply by binomial(n-k, k) */
        term *= binomial(n - k, k);

        /* accumulate the sum */
        result += term;
    }
    return result;
}
