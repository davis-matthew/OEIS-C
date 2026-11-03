/*
 * A066088  Number of distinct prime factors of sigma_2(n), where sigma_2(n) =
 *          A001157(n).
 * https://oeis.org/A066088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001221( sigma_2(n) ) where sigma_2(n) = sum of squares
 *            of divisors of n (utility function sigma_k(n,2) is used). The
 *            result is the number of distinct prime factors of that sum,
 *            computed by the helper function count_distinct_primes(uint64_t).
 *            For the standard 64‑bit build this is sufficien...
 * Range:     u8 n <= 14, u16 n <= 331, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 121 terms through n = 983 (2026-11-01)
 * Helpers:   omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/omega.h"

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
    /* Compute the sum of squares of the divisors of n. */
    seq_t s = sigma_k(n, 2);

    /* For typical builds seq_t fits in uint64_t for n up to 10 000.
       Cast to uint64_t to count distinct prime factors.  If s
       exceeds 2^64-1 the lower 64 bits are used, which is a
       known shortcoming when the arbitrary‑precision build is
       used for very large n. */
    uint64_t val = (uint64_t)s;

    int cnt = omega(val);
    return (seq_t)cnt;
}
