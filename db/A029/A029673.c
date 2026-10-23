/*
 * A029673  Theta series of quadratic form with Gram matrix [ 2, 0, 1; 0, 4,
 *          2; 1, 2, 4 ].
 * https://oeis.org/A029673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 367, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A029673 – Theta series of the quadratic form with Gram matrix
 *
 *  a(n) counts the integer triples (x,y,z) satisfying
 *        2x^2 + 4y^2 + 4z^2 + 2xz + 4yz = 2·n .
 *
 *  The function enumerates all feasible pairs (x,y) and solves
 *  the resulting quadratic equation for z, using only integer
 *  arithmetic.  The computation uses 128‑bit intermediate
 *  values where necessary to avoid overflow.
 */

#include <stdint.h>

/* Integer square root for an unsigned 64‑bit integer.
   Returns the greatest u with u*u ≤ n. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * (__int128)mid;
        if (sq == n) return mid;
        if (sq < n)  low = mid + 1;
        else        high = mid - 1;
    }
    return high;
}

/* Integer square root for a non‑negative 128‑bit integer.
   Returns the greatest u with u*u ≤ n (n ≥ 0). */
static __int128 isqrt128(__int128 n)
{
    if (n <= 0) return 0;
    __int128 low  = 0;
    __int128 high = ((__int128)1 << 64) - 1;          /* sqrt(2^128‑1) */
    while (low <= high) {
        __int128 mid = (low + high) >> 1;
        __int128 sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)  low  = mid + 1;
        else         high = mid - 1;
    }
    return high;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    const __int128 target = (__int128)2 * (__int128)n;     /* 2·n */

    /* Bounds derived from the real minima of the quadratic form:
       |x| <= sqrt( (6/5)·n ),   |y| <= sqrt( (7/10)·n ). */
    idx_t maxx = isqrt_u64( (idx_t)(( (__int128)6 * n) / 5) );
    idx_t maxy = isqrt_u64( (idx_t)(( (__int128)7 * n) / 10) );

    for (int64_t x = -(int64_t)maxx; x <= (int64_t)maxx; ++x) {
        for (int64_t y = -(int64_t)maxy; y <= (int64_t)maxy; ++y) {
            /* Quadratic in z: 4z^2 + (2x+4y)z + (2x^2+4y^2 - target) = 0 */
            const __int128 a_coef = 4;
            const __int128 b_coef = (__int128)2 * x + (__int128)4 * y;
            const __int128 c_coef = (__int128)2 * x * x + (__int128)4 * y * y - target;

            const __int128 D = b_coef * b_coef - 16 * c_coef;  /* discriminant */
            if (D < 0) continue;

            const __int128 sqrtD = isqrt128(D);
            if (sqrtD * sqrtD != D) continue;                 /* not a perfect square */

            const __int128 negB = -b_coef;

            /* First root: (-b + sqrtD) / 8 */
            __int128 num = negB + sqrtD;
            if ((num & 7) == 0) ++count;                     /* divisible by 8 */

            /* Second root, if distinct */
            if (sqrtD != 0) {
                num = negB - sqrtD;
                if ((num & 7) == 0) ++count;
            }
        }
    }
    return count;
}
