/*
 * A195198  Characteristic function of squares or three times squares.
 * https://oeis.org/A195198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = 1 if n is a perfect square or 3 × a perfect square, else
 *            0. a(0) = 1 (0 is a square)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Test whether an unsigned integer is a perfect square.
   Uses a binary search on [0, 2^32] and 128‑bit multiplication
   to avoid overflow. */
static inline bool is_square_u64(idx_t n)
{
    idx_t lo = 0, hi = 1ULL << 32;   /* hi is exclusive, 2^32 > sqrt(2^64-1) */
    while (lo < hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return ((unsigned __int128)lo * lo == n);
}

seq_t a(idx_t n)
{
    if (is_square_u64(n))
        return (seq_t)1;
    if ((n % 3) == 0)
    {
        idx_t m = n / 3;
        if (is_square_u64(m))
            return (seq_t)1;
    }
    return (seq_t)0;
}
