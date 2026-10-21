/*
 * A214284  Characteristic function of squares or five times squares.
 * https://oeis.org/A214284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = 1 if n is a perfect square or 5 times a perfect square, 0
 *            otherwise
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for a 64‑bit unsigned integer using binary search
   with a quick initial estimate from the bit‑length of the number. */
static idx_t isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;

    /* initial guess: 1 << (floor(log2(n))/2) */
    int shift = 63 - __builtin_clzll(n);
    idx_t r = 1ULL << (shift >> 1);

    /* adjust upwards while (r+1)^2 <= n */
    while (1) {
        unsigned __int128 t = (unsigned __int128)(r + 1) * (r + 1);
        if (t > n) break;
        ++r;
    }
    /* adjust downwards if necessary (unlikely) */
    while ((unsigned __int128)r * r > n)
        --r;

    return r;
}

/* OEIS A214284 */
seq_t a(idx_t n)
{
    /* check for a perfect square */
    idx_t r = isqrt_u64(n);
    if ((unsigned __int128)r * r == n)
        return (seq_t)1;

    /* check for 5 × perfect square */
    if (n % 5 == 0) {
        idx_t m = n / 5;
        idx_t s = isqrt_u64(m);
        if ((unsigned __int128)s * s == m)
            return (seq_t)1;
    }

    return (seq_t)0;
}
