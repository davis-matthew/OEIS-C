/*
 * A017708  Denominator of sum of -22nd powers of divisors of n.
 * https://oeis.org/A017708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n^22 / gcd( sigma_22(n), n^22 )
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 7, u128 n <= 56,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 8154 (2026-11-01)
 * Helpers:   gcd, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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
    seq_t sigma = sigma_k(n, 22);     /* sum of d^22 over divisors of n   */
    seq_t n_pow = ipow((seq_t)n, 22); /* n^22                               */
    seq_t g = gcd(sigma, n_pow);     /* common factor between sigma and n^22 */
    return n_pow / g;                /* reduced denominator               */
}
