/*
 * A017696  Denominator of sum of -16th powers of divisors of n.
 * https://oeis.org/A017696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n^16 / gcd(n^16, sigma_16(n)) where sigma_16(n) =
 *            sum_{d|n} d^16
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 15, u128 n <= 255,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 8937 (2026-11-01)
 * Helpers:   gcd, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the provided helpers: sigma_k, pow_uint, and gcd. */
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
    /* Compute n^16.  pow_uint expects a 64‑bit base, our n fits. */
    seq_t n_pow = ipow_u64_idx(n, 16);

    /* Compute sigma_16(n). */
    seq_t sigma = sigma_k(n, 16);

    /* Reduce the fraction sigma / n^16.
       The denominator after reduction is n^16 / gcd(n^16, sigma). */
    seq_t g = gcd(n_pow, sigma);
    return n_pow / g;
}
