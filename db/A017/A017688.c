/*
 * A017688  Denominator of sum of -12th powers of divisors of n.
 * https://oeis.org/A017688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n^12 / gcd(sigma_12(n), n^12)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 40, u128 n <= 1625,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 9711 (2026-11-01)
 * Helpers:   gcd, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/ipow.h"

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
    /* sigma_12(n) = Σ_{d|n} d^12 */
    seq_t sigma12 = sigma_k(n, 12U);

    /* n^12 as a seq_t */
    seq_t n_pow12 = ipow((seq_t)n, (seq_t)12U);

    /* greatest common divisor */
    seq_t g = gcd(sigma12, n_pow12);

    /* denominator of σ12(n) / n^12 */
    return n_pow12 / g;
}
