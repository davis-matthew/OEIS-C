/*
 * A162665  a(n) = sigma_2(n) + 3 sigma(n) tau(n) + tau(n)^3.
 * https://oeis.org/A162665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma_2(n) + 3 * sigma(n) * tau(n) + tau(n)^3
 * Range:     u8 n <= 5, u16 n <= 167, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9923 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here sigma_2(n) = sum of squares of divisors of n,
   sigma(n)   = sum of divisors of n,
   tau(n)     = number of divisors of n. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"

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

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; a(0) is never requested, but guard anyway. */
    if (n == 0)
        return 0;

    /* Compute each component using the provided utilities. */
    seq_t sigma2 = sigma_k(n, 2);                 /* σ₂(n) */
    seq_t sigma  = sigma_of(n);                   /* σ(n)  */
    seq_t tau    = (seq_t)num_divisors(n);   /* τ(n)  */

    /* Final value: σ₂(n) + 3σ(n)τ(n) + τ(n)³ */
    seq_t term = sigma2
                 + 3 * sigma * tau
                 + tau * tau * tau;

    return term;
}
