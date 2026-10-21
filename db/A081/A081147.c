/*
 * A081147  First differences of A022839.
 * https://oeis.org/A081147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)*sqrt(5)) - floor(n*sqrt(5))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_isqrt_u128.h"

seq_t a(idx_t n)
{
    unsigned __int128 nn = (unsigned __int128)n;
    /* floor_n  = floor(n * sqrt(5))  = floor( sqrt(5 * n^2) ) */
    unsigned __int128 temp_n  = nn * nn;
    temp_n *= 5;
    unsigned __int128 floor_n = _isqrt_u128(temp_n);

    /* floor_np1 = floor((n+1) * sqrt(5)) */
    unsigned __int128 np1     = nn + 1;
    unsigned __int128 temp_np1 = np1 * np1;
    temp_np1 *= 5;
    unsigned __int128 floor_np1 = _isqrt_u128(temp_np1);

    unsigned __int128 diff = floor_np1 - floor_n;   /* 2 or 3 */
    return (seq_t)diff;
}
