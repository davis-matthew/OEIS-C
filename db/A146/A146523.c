/*
 * A146523  Binomial transform of A010685.
 * https://oeis.org/A146523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 5 * 2^(n-1) for n >= 1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
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
    seq_t val = pow2(n - 1);   /* 2^(n-1) */
    val = val * (seq_t)5;       /* multiply by 5 */
    return val;
}
