/*
 * A091260  Denominator of sigma_3(n)/sigma_2(n).
 * https://oeis.org/A091260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, frac
 * Formula:   a(n) = denominator of sigma_3(n)/sigma_2(n)
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 982 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t sigma_k(idx_t m, uint8_t k)
{
    seq_t res = (seq_t)1;
    idx_t n = m;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            idx_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }

            /* compute pk = p^k */
            seq_t pk = (seq_t)1;
            for (idx_t i = 0; i < k; ++i) pk *= (seq_t)p;

            /* sum_{i=0}^{exp} (p^k)^i = 1 + pk + pk^2 + ...   */
            seq_t term = (seq_t)0;
            seq_t pow   = (seq_t)1;
            for (idx_t i = 0; i <= exp; ++i) {
                term += pow;
                pow  *= pk;
            }
            res *= term;
        }
    }

    if (n > 1) {               /* remaining prime factor with exponent 1 */
        /* k is 1 or 3, exp = 1 */
        seq_t pk = (seq_t)1;
        for (idx_t i = 0; i < k; ++i) pk *= (seq_t)n;

        seq_t term = (seq_t)0;
        seq_t pow   = (seq_t)1;
        for (idx_t i = 0; i <= (idx_t)1; ++i) {
            term += pow;
            pow  *= pk;
        }
        res *= term;
    }

    return res;
}

seq_t a(idx_t n)
{
    seq_t sigma2 = sigma_k(n, 2);      /* σ₂(n) = sum of squares of divisors */
    seq_t sigma3 = sigma_k(n, 3);      /* σ₃(n) = sum of cubes of divisors */
    seq_t g = gcd(sigma3, sigma2);     /* greatest common divisor of σ₃ and σ₂ */
    return sigma2 / g;                 /* denominator of the reduced fraction σ₃/σ₂ */
}
