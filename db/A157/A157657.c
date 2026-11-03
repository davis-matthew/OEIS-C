/*
 * A157657  a(1) = 1, a(n) = -mu(n) for n >= 2.
 * https://oeis.org/A157657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 for n = 1, otherwise a(n) = -mu(n)
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Validated: standard, pass, 116 terms through n = 4403 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the provided mu_of utility. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    int mu = mu_of(n);
    return (seq_t)(-mu);
}
