/*
 * A017686  Denominator of sum of -11th powers of divisors of n.
 * https://oeis.org/A017686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of sum_{d|n} 1/d^11 = a(n) = n^11 / gcd(n^11,
 *            sigma_11(n))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 56, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 994 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma_11(n) = Σ_{d|n} d^11 */

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
    /* compute n^11 */
    seq_t n_pow = (seq_t)n;
    for (idx_t i = 1; i < 11; ++i) {
        n_pow *= (seq_t)n;
    }

    /* compute sigma_11(n) */
    seq_t sigma = sigma_k(n, 11);

    /* gcd of n^11 and sigma_11(n) */
    seq_t g = gcd(n_pow, sigma);

    /* denominator after reducing the fraction */
    return n_pow / g;
}
