/*
 * A066102  Number of distinct prime factors of sigma_4(n), the sum of the 4th
 *          powers of divisors of n.
 * https://oeis.org/A066102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = omega(sigma_4(n)) (number of distinct prime factors of
 *            σ₄(n))
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 305, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 121 terms through n = 919 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

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

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* safety for index 0, though not expected */

    /* Compute σ₄(n).  The result fits into 64 bits for n up to 10000. */
    seq_t sigma = sigma_k(n, 4);
    uint64_t sigma64 = (uint64_t)sigma;   /* cast for factorization */

    if (sigma64 <= 1) return 0;  /* σ₄(1) = 1 → no prime factors */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(sigma64, primes, exps, &cnt);
    return (seq_t)cnt;
}
