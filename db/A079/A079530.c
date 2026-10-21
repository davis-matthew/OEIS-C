/*
 * A079530  a(n) = phi(n) - ceiling(sqrt(n)).
 * https://oeis.org/A079530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = phi(n) - ceil(sqrt(n))
 * Range:     i8 n <= 147, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"   /* provides the function phi(idx_t n) */

static inline idx_t floor_sqrt_u64(idx_t n)
{
    /* returns floor(sqrt(n)) for 0 <= n <= UINT64_MAX */
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 32;          /* (2^32)^2 = 2^64 fits in __int128 */

    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= n) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;       /* hi is floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    seq_t ph = phi(n);                     /* Euler's totient φ(n) */
    idx_t fsq = floor_sqrt_u64(n);         /* floor sqrt(n) */
    seq_t ceil_sqrt = (seq_t)(((__int128)fsq * fsq == n) ? fsq : fsq + 1);
    return ph - ceil_sqrt;                 /* φ(n) - ceil(sqrt(n)) */
}
