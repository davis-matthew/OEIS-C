/*
 * A066087  a(n) = gcd(sigma(n), phi(n)) - gcd(sigma(rad(n)), phi(rad(n)));
 *          rad = A007947.
 * https://oeis.org/A066087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = gcd(sigma(n), phi(n)) - gcd(sigma(rad(n)), phi(rad(n)))
 * Range:     i8 n <= 59, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 133 terms through n = 9829 (2026-11-01)
 * Helpers:   gcd, radical, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/radical.h"
#include "../utils/totient.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t sigma_n = sigma_of(n);
    seq_t phi_n   = totient(n);
    seq_t g1      = gcd(sigma_n, phi_n);

    seq_t rad_n   = radical(n);
    idx_t rad_idx = (idx_t)rad_n;
    seq_t sigma_rad = sigma_of(rad_idx);
    seq_t phi_rad   = totient(rad_idx);
    seq_t g2        = gcd(sigma_rad, phi_rad);

    return g1 - g2;
}
