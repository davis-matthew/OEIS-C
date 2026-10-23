/*
 * A182394  Signs of differences of number of divisors function: a(n) =
 *          sign(d(n)-d(n-1)), cf. A000005.
 * https://oeis.org/A182394
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if d(n)>d(n-1), -1 if d(n)<d(n-1), 0 otherwise
 * Range:     i8 n <= 8793, i16 n <= 8793, i32 n <= 8793, i64 n <= 8793,
 *            i128 n <= 8793, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* d(n) is the divisor count function (A000005) */

#include <stddef.h>

/* Helper: compute d(n) for n >= 1 using trial division. */
static seq_t divisor_count(idx_t n)
{
    if (n <= 1) {
        /* d(0) is irrelevant; d(1) = 1. */
        return (seq_t)1;
    }
    idx_t m = n;
    seq_t result = 1;

    /* Handle factor 2 separately to allow odd stepping. */
    if (m % 2 == 0) {
        seq_t exp = 0;
        while (m % 2 == 0) {
            m /= 2;
            exp++;
        }
        result *= (exp + 1);
    }

    /* Check odd factors up to sqrt(m). */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            seq_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            result *= (exp + 1);
        }
    }

    /* If m > 1, it is a prime factor > sqrt(n). */
    if (m > 1) {
        result *= 2;   /* exponent 1 => multiplicand 2 */
    }

    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) {
        /* Offset 2: terms below are undefined; return 0. */
        return 0;
    }

    seq_t d_n    = divisor_count(n);
    seq_t d_prev = divisor_count(n - 1);

    if (d_n > d_prev) return 1;
    if (d_n < d_prev) return -1;
    return 0;
}
