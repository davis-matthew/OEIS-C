/*
 * A091260  Denominator of sigma_3(n)/sigma_2(n).
 * https://oeis.org/A091260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   a(n) = denominator of sigma_3(n)/sigma_2(n)
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sigma2 = sigma_k(n, 2);      /* σ₂(n) = sum of squares of divisors */
    seq_t sigma3 = sigma_k(n, 3);      /* σ₃(n) = sum of cubes of divisors */
    seq_t g = gcd(sigma3, sigma2);     /* greatest common divisor of σ₃ and σ₂ */
    return sigma2 / g;                 /* denominator of the reduced fraction σ₃/σ₂ */
}
