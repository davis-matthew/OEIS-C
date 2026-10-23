/*
 * A190008  a(n) = n + [n*r/t] + [n*s/t]; r=1, s=sin(Pi/3), t=csc(Pi/3).
 * https://oeis.org/A190008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n*sin(π/3)⌋ + ⌊n*(sin(π/3))²⌋ * with sin(π/3)=√3/2,
 *            (sin(π/3))²=3/4 * = n + ⌊n*√3/2⌋ + ⌊3n/4⌋
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned __int128 (returns floor(sqrt(x))) */
static inline seq_t isqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 low   = 0;
    unsigned __int128 high  = (unsigned __int128)1 << 64;   /* 2^64  > any sqrt needed */
    unsigned __int128 mid, mid2;

    while (low <= high) {
        mid  = (low + high) >> 1;
        mid2 = mid * mid;
        if (mid2 <= x) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return (seq_t)high;
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 for the OEIS offset */
    unsigned __int128 un = (unsigned __int128)n;
    seq_t term1 = (seq_t)n;                            /* n */
    seq_t term2 = (seq_t)((un * 3) / 4);                /* ⌊3n/4⌋ */

    /* ⌊n√3/2⌋  = ⌊⌊n√3⌋ / 2⌋  where ⌊n√3⌋ = ⌊√(3n²)⌋ */
    unsigned __int128 t = un * un * 3;                  /* 3 * n * n */
    seq_t floor_sqrt3 = isqrt_uint128(t);              /* ⌊n√3⌋ */
    seq_t term3 = floor_sqrt3 >> 1;                    /* ⌊⌊n√3⌋ / 2⌋ */

    unsigned __int128 sum = (unsigned __int128)term1
                          + (unsigned __int128)term2
                          + (unsigned __int128)term3;

    return (seq_t)sum;
}
