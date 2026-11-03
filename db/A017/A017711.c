/*
 * A017711  Numerator of sum of -24th powers of divisors of n.
 * https://oeis.org/A017711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = sigma_24(n) / gcd(sigma_24(n), n^24)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 978 (2026-11-01)
 * Helpers:   gcd, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerator of the reduced fraction sigma_24(n)/n^24            */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/ipow_idx.h"

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
    if (n == 0) return 0; /* undefined for 0, but protect guard */

    /* numerator = σ₍₂₄₎(n) */
    seq_t num = sigma_k(n, 24);

    /* denominator = n²⁴ */
    seq_t denom = ipow_idx((seq_t)n, 24);

    /* common divisor */
    seq_t g = gcd(num, denom);

    /* reduced numerator */
    return num / g;
}
