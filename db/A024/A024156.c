/*
 * A024156  Number of integer-sided triangles with sides a,b,c, a<b<c, a+b+c=n
 *          that are obtuse.
 * https://oeis.org/A024156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of integer‐sided obtuse triangles (a<b<c), with
 *            a+b+c = n. The function enumerates all possible largest sides c
 *            and counts suitable triples (a,b) efficiently
 * Range:     u8 n <= 134, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t uint64_isqrt(idx_t n)
{
    idx_t low = 0, high = ((idx_t)1 << 32);   /* sqrt(2^64-1) < 2^32 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n) low = mid;
        else high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* For n < 3 there can be no triangle */
    if (n < 3) return (seq_t)0;

    idx_t count = 0;

    /* c must satisfy n/3 < c < n/2 and be integer */
    idx_t c_start = n / 3 + 1;               /* ceil((n+2)/3) */
    idx_t c_max   = (n - 1) / 2;             /* floor((n-1)/2) */

    for (idx_t c = c_start; c <= c_max; ++c) {
        idx_t S = n - c;                     /* a + b */
        if (S < 2) continue;                /* need at least a=1,b=1 */

        /* a must be > n - 2c and < b < c  */
        idx_t a_low  = n > 2*c ? n - 2*c + 1 : 1;
        idx_t a_high = (S - 1) / 2;
        if (a_low > a_high) continue;

        /* Check for existence of any obtuse at this c */
        if (S * S > 2 * c * c) continue;    /* 2c^2 <= S^2  → no obtuse */

        /* Δ = 2c^2 - S^2   (positive by previous test) */
        uint64_t delta = 2 * c * c - S * S;

        /* Compute integer sqrt of delta */
        idx_t s = uint64_isqrt(delta);
        idx_t d = S - s;                    /* positive as shown below */

        /* floor_r1 = floor( (S - sqrt(delta)) / 2 )   */
        idx_t floor_r1;
        if ((d & 1) == 0) {                /* d even */
            if (s * s == delta)            /* perfect square */
                floor_r1 = d / 2;
            else
                floor_r1 = d / 2 - 1;
        } else {                           /* d odd */
            floor_r1 = d / 2;              /* floor division */
        }

        /* a must satisfy a > r1, i.e. a >= floor_r1 + 1 */
        idx_t a_min = a_low;
        idx_t tmp   = floor_r1 + 1;
        if (tmp > a_min) a_min = tmp;
        if (a_min > a_high) continue;

        count += a_high - a_min + 1;
    }

    return (seq_t)count;
}
