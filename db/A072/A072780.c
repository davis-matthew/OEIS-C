/*
 * A072780  a(n) = sigma_2(n) + phi(n) * sigma(n) - 2*n^2, which is A072779(n)
 *          - 2*n^2.
 * https://oeis.org/A072780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = sigma_2(n) + phi(n) * sigma(n) - 2*n^2
 * Range:     u8 n <= 31, u16 n <= 431, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 91 terms through n = 978 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A072780 */

#include <stdint.h>
#include <stdlib.h>
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

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* sigma_2(n) */
    seq_t sigma2 = sigma_k(n, 2);

    /* sigma(n) – sum of divisors */
    seq_t sigma_n = sigma_of(n);

    /* phi(n) – Euler's totient */
    seq_t phi_n = totient(n);

    /* 2 * n^2 calculated in seq_t to avoid overflow. */
    seq_t n_sq   = (seq_t)n * (seq_t)n;
    seq_t two_n_sq = n_sq * 2;

    /* a(n) formula */
    return sigma2 + phi_n * sigma_n - two_n_sq;
}
