/*
 * A166234  The inverse of the constant 1 function under the exponential
 *          convolution (also called the exponential Möbius function).
 * https://oeis.org/A166234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, sign
 * Formula:   a(n) = ∏_{p^e || n} μ(e) where μ is the Möbius function
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small helper: compute μ(d) for a non‑negative integer d (d ≤ 64). */
static int mu_small(idx_t d) {
    int mu = 1;
    for (idx_t p = 2; p <= d / p; ++p) {
        if (d % p == 0) {
            d /= p;
            if (d % p == 0) {
                return 0;           /* square factor */
            }
            mu = -mu;                 /* flip sign per distinct factor */
        }
    }
    if (d > 1) {
        mu = -mu;                     /* remaining prime factor */
    }
    return mu;
}

seq_t a(idx_t n) {
    if (n == 1) {
        return 1;
    }

    seq_t result = 1;
    idx_t m = n;

    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            int mu = mu_small(e);
            result *= (seq_t)mu;
            if (result == 0) {
                return 0;            /* zero factor makes the whole product zero */
            }
        }
    }

    /* if remaining factor > 1, exponent is 1; μ(1)=1 so no effect */
    return result;
}
