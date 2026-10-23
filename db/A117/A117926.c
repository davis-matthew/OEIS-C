/*
 * A117926  a(n) = n^floor(sqrt(n)).
 * https://oeis.org/A117926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^floor(sqrt(n))
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 40, u64 n <= 99, u128 n <= 255,
 *            big any n
 * Helpers:   _floor_isqrt_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    idx_t e = _floor_isqrt_u64(n);   /* floor(sqrt(n)) */
    return seq_pow((seq_t)n, e);     /* n^e */
}
