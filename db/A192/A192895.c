/*
 * A192895  A000120-deficiency of n.
 * https://oeis.org/A192895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, 1 ≤ d < n} popcount(d) - popcount(n)
 * Range:     i8 n <= 2519, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* popcount for a 64‑bit unsigned integer (works for all n ≥ 1). */

#include <stdint.h>

/* Compute the binary weight of a 64‑bit integer. */
static inline int popcnt_uint64(uint64_t x)
{
    int c = 0;
    while (x) {
        x &= x - 1;   /* clear lowest set bit */
        ++c;
    }
    return c;
}

/*  OEIS A192895: a(n) = sum of popcounts of proper divisors of n minus popcount(n). */
seq_t a(idx_t n)
{
    seq_t sum = 0;                     /* running total of popcounts of proper divisors */
    if (n < 2) {
        /* For n = 1: no proper divisors, return -popcount(1) = -1. */
        return (seq_t)-popcnt_uint64((uint64_t)n);
    }

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* d is a divisor */
            if (d < n) {
                sum += (seq_t)popcnt_uint64(d);
            }

            idx_t other = n / d;
            if (other != d && other < n) {
                sum += (seq_t)popcnt_uint64(other);
            }
        }
    }

    sum -= (seq_t)popcnt_uint64((uint64_t)n);  /* subtract popcount(n) */
    return sum;
}
