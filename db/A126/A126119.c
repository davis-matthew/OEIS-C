/*
 * A126119  Numerators of sequence of fractions with e.g.f. (1+x)/(1-x)^(3/2).
 * https://oeis.org/A126119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (4*n+1)*((2*n)!)/(n!*2^n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t k = 1; k <= n; ++k) {
        prod *= (seq_t)(2 * k - 1);   /* multiply by odd numbers 1,3,5,...,2n-1 */
    }
    return prod * ((seq_t)(4 * n + 1));
}
