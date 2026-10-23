/*
 * A103452  Inverse of number triangle A103451.
 * https://oeis.org/A103452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = 1 if n+1 is a triangular number, * -1 if n is a
 *            triangular number, * 0 otherwise. * Triangular numbers are of
 *            the form k(k+1)/2, k>0
 * Range:     i8 n <= 9398, i16 n <= 9398, i32 n <= 9398, i64 n <= 9398,
 *            i128 n <= 9398, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square root: floor(sqrt(n)) for a 64‑bit unsigned integer. */
static idx_t isqrt(idx_t n)
{
    idx_t result = 0;
    /* Iterate bits from high to low. */
    for (int i = 63; i >= 0; --i) {
        idx_t bit = 1ULL << i;
        idx_t test = result | bit;
        /* Use 128‑bit arithmetic to avoid overflow during the test. */
        unsigned __int128 cand = (unsigned __int128)test * (unsigned __int128)test;
        if (cand <= n)
            result = test;
    }
    return result;
}

/* Check whether n (>0) is a triangular number.  0 is not considered triangular. */
static bool is_triangular(idx_t n)
{
    if (n == 0)
        return false;

    idx_t d = 1 + 8 * n;          /* discriminant of k(k+1)/2 = n */
    idx_t r = isqrt(d);
    if (r * r != d)
        return false;             /* not a perfect square -> not triangular */

    /* For a triangular number r must be odd; then (r-1)/2 is integer. */
    return ((r - 1) & 1) == 0;
}

seq_t a(idx_t n)
{
    if (is_triangular(n + 1))
        return (seq_t)1;
    if (is_triangular(n))
        return (seq_t)-1;
    return (seq_t)0;
}
