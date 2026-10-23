/*
 * A143433  Expansion of f(-x, x^3) in powers of x where f(,) is Ramanujan's
 *          general theta function.
 * https://oeis.org/A143433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 0 if n is not a triangular number; a(n) = (-1)^(k +
 *            floor(k/4)) where k satisfies k(k+1)/2 = n
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for 64 bit unsigned values: floor(sqrt(x)). */
static inline uint64_t uint64_isqrt(uint64_t x)
{
    uint64_t r = 0;
    for (int bit = 60; bit >= 0; bit--)
    {
        uint64_t cand = r | (1ULL << bit);
        if ((unsigned __int128)cand * cand <= x)
            r = cand;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Compute d = 1 + 8*n; the argument of the square root. */
    uint64_t d = 8ULL * n + 1;

    /* Integer square root of d. */
    uint64_t sq = uint64_isqrt(d);
    if ((unsigned __int128)sq * sq != d)
        return 0;               /* n is not a triangular number */

    /* k = (sqrt(d) - 1) / 2.  */
    uint64_t k = (sq - 1) / 2;

    /* Sign = (-1)^(k + floor(k/4)).  Compute parity of the exponent. */
    uint64_t exponent = k + (k >> 2);   /* k + floor(k/4) */
    int sign = (exponent & 1) ? -1 : 1;

    return (seq_t)sign;
}
