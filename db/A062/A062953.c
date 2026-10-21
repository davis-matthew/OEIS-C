/*
 * A062953  Multiplicative with a(p^e) = -p.
 * https://oeis.org/A062953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, sign, easy
 * Formula:   a(n) = ∏_{p|n} (−p) (multiplicative, a(p^e)=−p)
 * Range:     i8 n <= 128, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) is defined to be 1. */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return 1;               /* a(1) = 1 */

    seq_t result = 1;
    uint64_t m = n;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            result *= -(seq_t)p;        /* multiply by −p once per distinct prime */
            while (m % p == 0) m /= p;  /* strip all powers of p */
        }
    }
    if (m > 1) result *= -(seq_t)m;     /* remaining prime factor */

    return result;
}
