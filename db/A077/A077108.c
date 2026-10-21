/*
 * A077108  Duplicate of A075847.
 * https://oeis.org/A077108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n^2 – floor(cuberoot(n^2))^3
 * Range:     i8 n <= 21, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Return ⌊∛x⌋ for a non‐negative __int128 value x. */
static idx_t _floor_cuberoot(__int128 x) {
    if (x <= 0) return 0;
    /* initial approximation using long double */
    long double dx = (long double)x;
    uint64_t r = (uint64_t)floor(cbrt(dx));

    /* correct possible off–by–one errors */
    while ((__int128)r * r * r > x) --r;
    while (((__int128)(r + 1) * (r + 1) * (r + 1)) <= x) ++r;

    return (idx_t)r;
}

seq_t a(idx_t n) {
    /* n^2 as a 128‑bit integer to avoid overflow */
    __int128 nsq128 = (__int128)n * (__int128)n;

    /* compute ⌊∛(n^2)⌋ and its cube */
    idx_t r = _floor_cuberoot(nsq128);
    __int128 cube128 = (__int128)r * (__int128)r * (__int128)r;

    /* a(n) = n^2 – r^3 */
    return (seq_t)(nsq128 - cube128);
}
