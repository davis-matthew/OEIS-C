/*
 * A060150  a(0) = 1; for n > 0, binomial(2n-1, n-1)^2.
 * https://oeis.org/A060150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(0)=1; for n>0, a(n) = binomial(2*n-1, n-1)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute C(2*n-1, n-1) once and square it. */
    seq_t c = binomial((idx_t)(2 * n - 1), (idx_t)(n - 1));
    return c * c;
}
