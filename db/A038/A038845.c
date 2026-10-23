/*
 * A038845  3-fold convolution of A000302 (powers of 4).
 * https://oeis.org/A038845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+2,2) * 4^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    // Compute binomial(n+2, 2) = (n+2)(n+1)/2 exactly.
    seq_t bin = ((seq_t)(n + 2)) * ((seq_t)(n + 1));
    bin /= 2;                 // exact division
    // Compute 4^n via the utility function.
    seq_t p4 = pow4(n);
    // Result is bin * 4^n.
    return bin * p4;
}
