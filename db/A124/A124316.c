/*
 * A124316  a(n) = Sum_{d|n} sigma(gcd(d,n/d)), where sigma is the sum of
 *          divisors function, A000203.
 * https://oeis.org/A124316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} sigma(gcd(d, n/d))
 * Range:     u8 n <= 12, u16 n <= 2227, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute p^exp for a uint64_t base and an index type exponent. */
static inline seq_t pow_seq_uint(uint64_t base, idx_t exp) {
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)base;
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset starts at 1 */

    /* Factorise n – works for all n up to a few million */
    uint64_t primes[64];
    int       exps[64];
    idx_t     cnt = 0;
    uint64_t  tmp = (uint64_t)n;
    uint64_t  p   = 2;
    int       e;

    while (p * p <= tmp) {
        e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        if (e > 0) {
            primes[cnt] = p;
            exps[cnt++] = e;
        }
        p = (p == 2) ? 3 : p + 2;    /* after 2, only odd primes */
    }
    if (tmp > 1) {                   /* remaining prime factor */
        primes[cnt] = tmp;
        exps[cnt++] = 1;
    }

    seq_t result = 1;
    for (idx_t i = 0; i < cnt; ++i) {
        uint64_t prime = primes[i];
        int      exp   = exps[i];

        seq_t numerator;
        if ((exp & 1) == 0) { /* even exponent */
            uint64_t k = exp / 2;
            seq_t   p_pow = pow_seq_uint(prime, k + 1);   /* p^(k+1) */
            numerator = p_pow * ((seq_t)prime + 1);
            numerator -= (seq_t)(exp + 3) * (seq_t)prime;
            numerator += (seq_t)(exp + 1);
        } else {            /* odd exponent */
            uint64_t t = (exp + 3) / 2;
            seq_t   p_pow = pow_seq_uint(prime, t);   /* p^t */
            numerator = (seq_t)2 * p_pow;
            numerator -= (seq_t)(exp + 3) * (seq_t)prime;
            numerator += (seq_t)(exp + 1);
        }

        seq_t denom = (seq_t)(prime - 1) * (seq_t)(prime - 1);
        seq_t term  = numerator / denom;   /* integer division, exact */
        result *= term;
    }

    return result;
}
