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
 * Helpers:   gcd, phi, rad, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/phi.h"
#include "../utils/rad.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t sigma_n = sigma_of(n);
    seq_t phi_n   = phi(n);
    seq_t g1      = gcd(sigma_n, phi_n);

    seq_t rad_n   = rad(n);
    idx_t rad_idx = (idx_t)rad_n;
    seq_t sigma_rad = sigma_of(rad_idx);
    seq_t phi_rad   = phi(rad_idx);
    seq_t g2        = gcd(sigma_rad, phi_rad);

    return g1 - g2;
}
