/*
 * A051248  n^a(n) is the smallest power of n (with a(n) > 1) which starts
 *          with n.
 * https://oeis.org/A051248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) is the minimal integer k > 1 such that n^k begins with the
 *            decimal digits of n. For n = 1, by convention, a(1) = 2. The
 *            function uses the fractional part of k·log10(n) to test whether
 *            the leading digits match n
 * Range:     u8 n <= 26, u16 n <= 275, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Helper: count decimal digits of an unsigned 64‑bit integer. */
static int digits_uint64(uint64_t x)
{
    int d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;

    /* Compute log10(n) and log10(n+1) as long double for high precision. */
    long double log_n     = log10l((long double)n);
    long double log_nplus = log10l((long double)(n + 1));

    /* Number of decimal digits of n. */
    int dig = digits_uint64((uint64_t)n);

    /* Interval for the fractional part of k·log10(n). */
    long double A = log_n - (dig - 1);                  /* inclusive lower bound */
    long double B = log_nplus - (dig - 1);               /* exclusive upper bound */

    /* Increment step = fractional part of log10(n). */
    long double inc = log_n;
    inc -= floorl(inc);    /* keep only fractional part */

    /* Start with k = 1, compute its fractional part. */
    long double f = inc;
    seq_t k = 2;           /* we are looking for exponents > 1 */

    const long double eps = 1e-15L; /* safety margin for rounding */
    while (1) {
        /* Update fractional part for next k. */
        f += inc;
        if (f >= 1.0L) f -= 1.0L;

        /* Check if it lies in the required interval. */
        if (f + eps >= A && f < B - eps) {
            return (seq_t)k;
        }
        ++k;
    }
}
