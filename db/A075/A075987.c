/*
 * A075987  Numerator(1+1/prime(1)^3+ ... + 1/prime(n)^3) where prime(k) is
 *          the k-th prime.
 * https://oeis.org/A075987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n>0, a(n) = a(n-1)*prime(n)^3 +
 *            (product_{k=1}^{n-1}prime(k))^3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   seq_mul, seq_pow, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* external utilities for primes and big integer arithmetic */
#include "../utils/sieve_first_n_primes.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* compute the n-th term of OEIS A075987 */
seq_t a(idx_t n) {
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* number of slots allocated (cache_len-1 is last index stored) */
    static seq_t prod_cum = 1;            /* product of primes up to current index (product of primes[0..i-1]) */
    static size_t prod_len = 0;           /* number of primes processed so far (i.e., prod_len == last index processed) */
    static uint32_t *primes = NULL;       /* array of first primes (indexing from 0) */
    static size_t primes_cnt = 0;         /* number of primes stored */

    if (n == 0) {
        return (seq_t)1;
    }

    /* Ensure we have all primes up to the requested index */
    if (primes_cnt < (size_t)n) {
        free(primes);
        sieve_first_n_primes((idx_t)n, &primes, &primes_cnt);
    }

    /* Expand cache if needed */
    if (cache_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed; caller may handle */
        /* initialize first element */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache_len = 1;
            prod_cum = 1;
            prod_len = 0;
        }
        cache_len = new_len;
    }

    /* Compute any missing terms up to n */
    for (size_t i = prod_len; i < (size_t)n; ++i) { /* i is zero‑based index for prime(i+1) */
        /* prime at position i+1 is primes[i] */
        seq_t p = (seq_t)primes[i];
        seq_t p3 = seq_pow(p, 3);
        seq_t prod3 = seq_pow(prod_cum, 3);
        seq_t a_prev = cache[i];
        cache[i + 1] = a_prev * p3 + prod3;
        prod_cum = seq_mul(prod_cum, p);
        prod_len = i + 1;
    }

    return cache[(size_t)n];
}
