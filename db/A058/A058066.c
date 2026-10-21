/*
 * A058066  a(n) = floor(n*t), t = 1 + sqrt(5)/2.
 * https://oeis.org/A058066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * (1 + sqrt(5)/2))
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* integer square root of a 128‑bit unsigned integer */
static unsigned __int128 int128_sqrt(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = (unsigned __int128)1u << 64; /* 2^64 */
    while (low < high) {
        unsigned __int128 mid = (low + high + 1) >> 1;
        if (mid * mid <= x) low = mid;
        else high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Special case */
    if (n == 0) return 0;

    /* Compute floor( sqrt(5 * n * n) ) using 128‑bit arithmetic */
    unsigned __int128 prod = (unsigned __int128)n * n * (unsigned __int128)5;
    unsigned __int128 sq = int128_sqrt(prod);

    /* floor( sqrt(5 * n * n) / 2 ) = sq / 2 */
    unsigned __int128 half_sq = sq >> 1;

    /* a(n) = n + floor( sqrt(5 * n * n) / 2 ) */
    unsigned __int128 result = (unsigned __int128)n + half_sq;

    return (seq_t)result;
}
