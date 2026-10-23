/*
 * A138392  Hankel transform of A062992 with interpolated zeros.
 * https://oeis.org/A138392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(comb(n+1,2)+0^n-1)*(2+floor((n+1)/2)-0^n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t exp = (seq_t)n * ((seq_t)n + 1) / 2 - 1;
    seq_t pv  = pow2((idx_t)exp);
    unsigned int factor = 2 + (unsigned int)((n + 1) / 2);
    return mul_small(pv, factor);
}
