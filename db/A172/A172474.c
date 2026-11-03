/*
 * A172474  a(n) = floor(n*sqrt(2)/4).
 * https://oeis.org/A172474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(2)/4)
 * Range:     u8 n <= 724, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 92 terms through n = 988 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compare mid^2 <= X where X = 2*n^2 (represented as 128-bit low and one high bit) */
static int
le_x(unsigned __int128 mid, int x_high, unsigned __int128 x_low)
{
    uint64_t hi = (uint64_t)(mid >> 64);      /* 0 or 1 */
    uint64_t lo = (uint64_t)mid;

    /* lo^2 */
    unsigned __int128 lo_sq = (unsigned __int128)lo * lo;

    /* cross term: 2*hi*lo */
    unsigned __int128 cross = (unsigned __int128)hi * lo; /* equals lo if hi==1 else 0 */
    unsigned __int128 double_cross = cross << 1;         /* up to 65 bits */

    /* separate into low and high parts of cross*2^64 */
    unsigned __int128 cross_low = ((unsigned __int128)(uint64_t)double_cross) << 64;
    unsigned int   cross_high = (unsigned int)(double_cross >> 64); /* 0 or 1 */

    /* low part of product */
    unsigned __int128 prod_low = lo_sq + cross_low;
    unsigned int carry = (prod_low < lo_sq) ? 1u : 0u;

    /* high part of product */
    unsigned int prod_high = (unsigned int)(hi * hi) + cross_high + carry; /* 0 .. 3 */

    if (prod_high < (unsigned int)x_high) return 1;
    if (prod_high > (unsigned int)x_high) return 0;
    return prod_low <= x_low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* 2*n^2 represented as high bit + 128-bit low part */
    unsigned __int128 n128 = (unsigned __int128)n;
    unsigned __int128 sq = n128 * n128;                 /* n^2 (0 .. 2^128-1) */
    int          x_high = (int)((sq >> 127) & 1ULL);    /* bit 128 of 2*n^2 */
    unsigned __int128 x_low  = (unsigned __int128)sq << 1; /* low 128 bits of 2*n^2 */

    /* binary search for largest k with k^2 <= 2*n^2 */
    unsigned __int128 low  = 0;
    unsigned __int128 high = (unsigned __int128)n * 2 + 1;   /* exclusive upper */

    while (low + 1 < high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (le_x(mid, x_high, x_low))
            low = mid;
        else
            high = mid;
    }

    /* a(n) = floor(floor(n*sqrt(2)) / 4) = floor(low / 4) */
    return (seq_t)(low >> 2);
}
