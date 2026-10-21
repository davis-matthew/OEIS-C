/*
 * A095185  a(n) = least k such that k*n^(1/2) differs from an integer by less
 *          than 1/n.
 * https://oeis.org/A095185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that k * sqrt(n) differs from an integer by
 *            < 1/n
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by iterating k until the fractional part of k*sqrt(n) is within 1/n */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is at least 1 */
    /* Compute real square root of n with high precision */
    long double sqrt_n = sqrtl((long double)n);

    /* Special case: perfect square -> answer 1 */
    if ((n == 0) || (sqrt_n == (long double)((long long)sqrt_n))) {
        return (seq_t)1;
    }

    /* 1/n as a long double */
    long double eps = 1.0L / (long double)n;

    /* Iterate k from 1 upwards */
    for (idx_t k = 1;; ++k) {
        long double val = (long double)k * sqrt_n;
        /* nearest integer to val */
        idx_t nearest = (idx_t)floor(val + 0.5L);
        long double diff = fabsl(val - (long double)nearest);
        if (diff < eps) {
            return (seq_t)k;
        }
    }
}
