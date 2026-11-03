/*
 * A079539  a(n) = sigma[k](n) - phi(n)^k - d(n)^k for k=3.
 * https://oeis.org/A079539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sigma_k(n,3) - phi(n)^3 - d(n)^3
 * Range:     i8 n <= 5, i16 n <= 31, i32 n <= 1229, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9635 (2026-11-01)
 * Helpers:   num_divisors, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_divisors.h"
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

/* Compute A079539(a(n)) */
seq_t a(idx_t n)
{
    /* Sum of cubes of divisors */
    seq_t sigma3 = sigma_k(n, 3);

    /* Euler's totient */
    seq_t ph = totient(n);

    /* Cube of the totient */
    seq_t ph3 = ph * ph * ph;

    /* Number of divisors */
    seq_t d = (seq_t)num_divisors((uint64_t)n);

    /* Cube of the divisor count */
    seq_t d3 = d * d * d;

    /* Final result */
    return sigma3 - ph3 - d3;
}
