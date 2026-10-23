/*
 * A188086  [nr+kr]-[nr]-[kr], where r=sqrt(3), k=3, [ ]=floor.
 * https://oeis.org/A188086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+3)*sqrt(3)) - floor(n*sqrt(3)) -
 *            floor(3*sqrt(3))
 * Range:     u8 n <= 138, u16 n <= 138, u32 n <= 138, u64 n <= 138,
 *            u128 n <= 138, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* floor(3*sqrt(3)) = 5 */
#include <stdint.h>
#include <stdlib.h>

/* integer square root of an unsigned __int128 (floor(sqrt(x))) */
static inline unsigned long long isqrt128(unsigned __int128 x)
{
    unsigned long long lo = 0, hi = (unsigned long long)1ULL << 63;
    unsigned long long ans = 0;
    while (lo <= hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

/* floor(n * sqrt(3))  as in the OEIS definition */
static seq_t floor_sqrt3(idx_t n)
{
    unsigned __int128 prod = (unsigned __int128)3 * n * n;  /* 3*n^2 */
    return (seq_t)isqrt128(prod);
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for 0, but safe */

    seq_t v_np3 = floor_sqrt3(n + 3);   /* floor((n+3)*sqrt(3))  */
    seq_t v_n   = floor_sqrt3(n);       /* floor(n*sqrt(3))       */
    return v_np3 - v_n - 5;            /* subtract floor(3*sqrt3)=5 */
}
