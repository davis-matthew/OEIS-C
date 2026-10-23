/*
 * A184920  a(n) = n+[s*n/r]+[t*n/r]+[u*n/r], where []=floor and r=2^(1/2),
 *          s=r+1, t=r+2, u=r+3.
 * https://oeis.org/A184920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + ⌊n/√2⌋ + ⌊n√2⌋ + ⌊3n/√2⌋
 * Range:     u8 n <= 31, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* All arithmetic is performed using unsigned __int128 to avoid overflow. */
/* No floating-point operations are used. */

/* Integer square root of an unsigned __int128: */
/* Returns ⌊√x⌋ for 0 ≤ x < 2^127. */
static inline __int128 isqrt128(__int128 x)
{
    __int128 lo = 0;
    __int128 hi = (( (__int128)1) << 64);  /* 2^64 > sqrt(max 128-bit) */
    while (lo < hi) {
        __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Compute n^2 using 128-bit intermediate */
    __int128 n_sq = (__int128)n * n;

    /* ⌊n/√2⌋ = ⌊√(2*n^2)⌋ / 2  */
    __int128 sqrt2n = isqrt128((__int128)2 * n_sq);
    seq_t floor_n_over_r = (seq_t)(sqrt2n / 2);

    /* ⌊n√2⌋ = ⌊√(2*n^2)⌋  */
    seq_t floor_2n_over_r = (seq_t)sqrt2n;

    /* ⌊3n/√2⌋ = ⌊√(18*n^2)⌋ / 2  */
    __int128 sqrt18n = isqrt128((__int128)18 * n_sq);
    seq_t floor_3n_over_r = (seq_t)(sqrt18n / 2);

    /* Assemble the result */
    __int128 sum = (__int128)4 * n;
    sum += (__int128)floor_n_over_r;
    sum += (__int128)floor_2n_over_r;
    sum += (__int128)floor_3n_over_r;

    return (seq_t)sum;
}
