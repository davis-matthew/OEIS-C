/*
 * A170735  Expansion of g.f.: (1+x)/(1-15*x).
 * https://oeis.org/A170735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = 16 * 15^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the big exponent helper */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute 15^(n-1) in the same type as the sequence values. */
    seq_t pow15 = big_pow((seq_t)15, (seq_t)(n - 1));

    /* Multiply by 16 to get the final term. */
    return pow15 * (seq_t)16;
}
