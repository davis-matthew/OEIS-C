/*
 * A081129  Differences of Beatty sequence for cube root of 3.
 * https://oeis.org/A081129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1) * 3^(1/3)) – floor(n * 3^(1/3))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   icbrt_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/icbrt_uint128.h"

seq_t a(idx_t n)
{
    /* floor(n * 3^(1/3))  == icbrt_uint128(3 * n^3) */
    unsigned __int128 n128 = n;
    unsigned __int128 v1 = 3 * n128 * n128 * n128;   /* 3 * n^3 */
    seq_t f = icbrt_uint128(v1);

    idx_t n1 = n + 1;
    unsigned __int128 n1_128 = n1;
    unsigned __int128 v2 = 3 * n1_128 * n1_128 * n1_128; /* 3 * (n+1)^3 */
    seq_t f1 = icbrt_uint128(v2);

    return f1 - f;
}
