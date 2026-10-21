/*
 * A084858  Binomial transform of A001651.
 * https://oeis.org/A084858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = 3*(n+1)*2^n / 4
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t pow2n = pow2((idx_t)n);
    seq_t tmp   = ((seq_t)(n + 1) * pow2n) / ((seq_t)4);
    return tmp * ((seq_t)3);
}
