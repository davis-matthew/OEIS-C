/*
 * A213569  Principal diagonal of the convolution array A213568.
 * https://oeis.org/A213569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*2^n - 2*n - 1
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pwr = pow2(n);
    seq_t term1 = ((seq_t)n + 1) * pwr;
    seq_t term2 = (seq_t)(2 * n);
    seq_t res = term1 - term2 - 1;
    return res;
}
