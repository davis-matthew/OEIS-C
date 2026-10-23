/*
 * A190506  n+[nr/t]+[ns/t]+[nu/t]; r=golden ratio, s=r+1, t=r+2, u=r+3.
 * https://oeis.org/A190506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n - 1 + floor(n*sqrt(5)/5)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   _int128_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_int128_isqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* unsafe / unused, but harmless */

    /* n is idx_t (uint64_t); cast to 128‑bit for intermediate products      */
    unsigned __int128 n128 = (unsigned __int128)n;
    unsigned __int128 prod = 5 * n128 * n128;      /* 5*n^2  (fits 128 bits for all n <= 2^64-1) */

    unsigned __int128 sqrt_prod = _int128_isqrt(prod);   /* floor(sqrt(5*n^2))   */

    unsigned __int128 term = sqrt_prod / 5;               /* floor(n*sqrt5/5) */

    unsigned __int128 result = 3 * n128 - 1 + term;        /* a(n) = 3n-1 + term   */

    return (seq_t)result;                                 /* narrow to seq_t        */
}
