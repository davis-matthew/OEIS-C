/*
 * A172474  a(n) = floor(n*sqrt(2)/4).
 * https://oeis.org/A172474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(2)/4)
 * Range:     u8 n <= 724, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   le_x
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/le_x.h"

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
