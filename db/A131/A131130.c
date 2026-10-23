/*
 * A131130  Binomial transform of [1,1,7,1,7,1,7,1,...].
 * https://oeis.org/A131130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*2^n - 6 for n >= 1; a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                     /* a(0) = 1 */

    seq_t pow2n = pow_seq((seq_t)2, (int)n);   /* compute 2^n */
    seq_t val   = pow2n * 4;                   /* 4*2^n */
    return val - 6;                            /* subtract 6 */
}
