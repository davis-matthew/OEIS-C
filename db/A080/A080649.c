/*
 * A080649  Sum of prime factors of sigma(n).
 * https://oeis.org/A080649
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct prime factors of σ(n) (σ is the sum of
 *            divisors)
 * Range:     u8 n <= 288, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_distinct_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine factors n by trial division, computes σ(n) with the multiplicative
   formula, and then factors σ(n) again to add its distinct prime divisors. */

#include <stdint.h>
#include "../utils/sum_distinct_prime_factors.h"

/* ----- Helper: compute σ(n) ------------------------------------------- */
static idx_t sigma_of(idx_t n)
{
    if (n < 2) return n;          /* σ(1) = 1, handled by caller for n ≥ 2 */

    idx_t m = n;
    idx_t result = 1;

    for (idx_t p = 2; p <= m / p; ++p) {     /* p * p <= m  (avoids overflow) */
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            /* term = 1 + p + p² + … + p^exp  (sum of geometric series) */
            idx_t term = 1;
            idx_t power = 1;
            for (idx_t i = 0; i < exp; ++i) {
                power *= p;
                term += power;
            }
            result *= term;
        }
    }

    if (m > 1) {   /* m is prime */
        result *= (1 + m);
    }
    return result;
}

/* ----- Public sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* the sequence is defined for n ≥ 2 */

    idx_t sig = sigma_of(n);      /* σ(n) – fits in idx_t for the tested range */
    idx_t sum = sum_distinct_prime_factors(sig);

    /* Convert back to seq_t (the caller’s unsigned integer type). */
    return (seq_t)sum;
}
