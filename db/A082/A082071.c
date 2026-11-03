/*
 * A082071  Smallest common prime-divisor of phi(n) = A000010(n) and
 *          sigma_2(n) = A001157(n); a(n)=1 if no common prime-divisor exists.
 * https://oeis.org/A082071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime divisor of gcd(phi(n), sigma_2(n)), or 1
 *            if gcd = 1
 * Range:     u8 n <= 35, u16 n <= 5407, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9436 (2026-11-01)
 * Helpers:   gcd, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/totient.h"

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
    seq_t phi_val   = totient(n);          /* φ(n) */
    seq_t sigma2_val= sigma_k(n, 2);   /* σ₂(n) */
    seq_t g = gcd(phi_val, sigma2_val);

    if (g == 1) return 1;              /* no common prime divisor */

    /* g fits into uint64_t for the typical n-range; cast safely */
    uint64_t gv = (uint64_t)g;

    /* find the smallest prime factor by trial division */
    if (gv % 2 == 0) return 2;
    for (uint64_t d = 3; d * d <= gv; d += 2)
        if (gv % d == 0)
            return d;
    return (seq_t)gv;                  /* gv itself is prime */
}
