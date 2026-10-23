/*
 * A184653  floor(n*s+h-h*s), where s=(3+sqrt(3))/2, h=-1/2; complement of
 *          A184638.
 * https://oeis.org/A184653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*s + h - h*s), where s=(3+sqrt(3))/2, h=-1/2
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned __int128: floor(sqrt(n)) */
static unsigned __int128 int_sqrt_u128 (unsigned __int128 n) {
    unsigned __int128 low = 0, high = ((unsigned __int128)1 << 64);
    while (low < high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid * mid <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset is 1; no term for n==0 */

    /* Scale for rational comparisons (2^63) */
    static const unsigned __int128 SCALE = ((unsigned __int128)1) << 63;
    /* Cached sqrt(3) * 2^63 */
    static unsigned __int128 sqrt_scaled = 0;
    if (sqrt_scaled == 0) {
        /* sqrt_scaled = floor( sqrt(3) * 2^63 ) */
        unsigned __int128 big = ((unsigned __int128)3) << 126;   /* 3 * 2^126 */
        sqrt_scaled = int_sqrt_u128(big);
    }

    seq_t val = (seq_t)3;          /* a(1) */
    if (n == 1) return val;

    /* Maintain fractional part as (A + B*sqrt3)/4 with integer A,B */
    int64_t A = -5;                /* initially from r_1 = (3sqrt3 -5)/4 */
    int64_t B = 3;

    for (idx_t i = 1; i < n; ++i) {
        /* Determine whether r_n <= t where t = (3 - sqrt3)/2 */
        __int128 lhs = (__int128)(B + 2) * sqrt_scaled;   /* (B+2)*sqrt3_scaled */
        __int128 rhs = (__int128)(6 - A) * SCALE;         /* (6-A)*2^63 */
        int diff = (lhs <= rhs) ? 2 : 3;                  /* floor(alpha + r_n) */

        val += (seq_t)diff;           /* update a(n) */

        /* Update A and B for next term */
        B += 2;
        if (diff == 2)
            A -= 2;
        else
            A -= 6;                     /* diff==3 */
    }

    return val;
}
