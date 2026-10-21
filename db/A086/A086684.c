/*
 * A086684  a(n) = n! - Sum_{i} p_i!^e_i, where n = Product_{i} (p_i^e_i).
 * https://oeis.org/A086684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - Σ (p_i!)^{e_i} where n = Π (p_i^{e_i}) is the prime
 *            factorization of n
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* cached factorials, fact_cache[0] = 0! = 1. */
static seq_t *fact_cache = NULL;
static size_t fact_cache_size = 0;

/* Ensure factorial table is computed up to at least idx. */
static void ensure_fact_upto(idx_t idx)
{
    if (fact_cache_size <= idx) {
        size_t new_size = fact_cache_size ? fact_cache_size * 2 : 16;
        if (new_size <= idx) new_size = idx + 1;

        seq_t *tmp = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();      /* out of memory */
        fact_cache = tmp;

        if (fact_cache_size == 0) {
            fact_cache[0] = 1;         /* 0! = 1 */
            /* we will compute 1! next in the loop */
            fact_cache_size = 1;
        }

        /* compute missing factorials */
        for (size_t i = fact_cache_size; i < new_size; ++i) {
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        fact_cache_size = new_size;
    }
}

/* integer exponentiation for seq_t, base^exp.  exp >= 0. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;           /* 0! = 1, no prime factors → a(0)=1 (unused) */

    /* compute n! */
    ensure_fact_upto(n);
    seq_t fact_n = fact_cache[n];

    seq_t sum = 0;

    /* factorization of n by trial division */
    idx_t m = n;
    for (idx_t p = 2; (p * p) <= m; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* use factorial of p */
            if (p >= fact_cache_size) ensure_fact_upto(p);
            seq_t fac_p = fact_cache[(size_t)p];
            seq_t term = pow_seq_t(fac_p, exp);
            sum += term;
        }
    }
    if (m > 1) {
        /* remaining prime factor m^1 */
        if (m >= fact_cache_size) ensure_fact_upto(m);
        seq_t fac_m = fact_cache[(size_t)m];
        sum += fac_m;                  /* exp = 1, so (m!)^1 = m! */
    }

    return fact_n - sum;
}
