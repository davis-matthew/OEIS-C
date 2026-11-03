/*
 * A017706  Denominator of sum of -21st powers of divisors of n.
 * https://oeis.org/A017706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of sigma_21(n) / n^21 in lowest terms
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 8, u128 n <= 69,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 8870 (2026-11-01)
 * Helpers:   gcd, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/ipow_u64_idx.h"

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
    /* compute n^21 */
    seq_t n_pow = ipow_u64_idx((uint64_t)n, (idx_t)21);

    /* compute sigma_21(n) */
    seq_t sigma = sigma_k((idx_t)n, (uint8_t)21);

    /* reduce the fraction sigma / n_pow */
    seq_t g = gcd(n_pow, sigma);

    /* denominator of the reduced fraction */
    return n_pow / g;
}
