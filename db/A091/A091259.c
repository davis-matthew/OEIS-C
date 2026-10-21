/*
 * A091259  Numerator of sigma_3(n)/sigma(n).
 * https://oeis.org/A091259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, frac, look
 * Formula:   a(n) = numerator of sigma_3(n)/sigma(n)
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 1679, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, sigma_k, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/sigma_k.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* For n >= 1 the value of sigma_of and sigma_k are well defined. */
    seq_t sigma1 = sigma_of(n);   /* σ(n) = σ_1(n) */
    seq_t sigma3 = sigma_k(n, 3); /* σ_3(n) */
    seq_t g = gcd(sigma3, sigma1);
    return sigma3 / g;
}
