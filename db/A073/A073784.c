/*
 * A073784  Number of primes between successive composite numbers.
 * https://oeis.org/A073784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of primes strictly between the n‑th and (n+1)‑th composite numbers. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Global cached data */
static idx_t *composites      = NULL;   /* 0‑based array of composites */
static size_t composites_len  = 0;      /* number of composites stored */
static uint8_t *is_prime      = NULL;   /* 1 if prime, 0 otherwise */
static idx_t *prime_prefix    = NULL;   /* prime count ≤ i */
static idx_t limit            = 0;      /* current sieve upper bound */

/* Build or rebuild the sieve up to 'new_limit', recomputing all cached arrays. */
static void build_limit(idx_t new_limit) {
    size_t cap = (size_t)new_limit + 1;
    uint8_t *p = (uint8_t *)calloc(cap, 1);
    size_t i;

    for (i = 2; i <= (size_t)new_limit; ++i) p[i] = 1;
    for (i = 2; i * i <= (size_t)new_limit; ++i) {
        if (!p[i]) continue;
        for (size_t j = i * i; j <= (size_t)new_limit; j += i) p[j] = 0;
    }

    /* Free old arrays */
    free(is_prime);
    free(composites);
    free(prime_prefix);

    is_prime = p;

    /* Count composites and collect them */
    composites_len = 0;
    for (i = 4; i <= (size_t)new_limit; ++i) {
        if (!p[i]) composites_len++;
    }
    composites = (idx_t *)malloc(composites_len * sizeof(idx_t));
    size_t k = 0;
    for (i = 4; i <= (size_t)new_limit; ++i) {
        if (!p[i]) composites[k++] = (idx_t)i;
    }

    /* Compute prefix prime counts */
    prime_prefix = (idx_t *)malloc(cap * sizeof(idx_t));
    prime_prefix[0] = 0;
    for (i = 1; i <= (size_t)new_limit; ++i) {
        prime_prefix[i] = prime_prefix[i - 1] + (p[i] ? 1 : 0);
    }

    limit = new_limit;
}

/* Ensure that the cache holds at least 'needed' composites
   and already computed prime data for primes up to the needed composite. */
static void ensure_limit(size_t needed) {
    if (limit == 0) limit = 4;           /* initial sieve bound */
    while (composites_len <= needed) {   /* grow until enough composites */
        limit *= 2;
        build_limit(limit);
    }
}

/* Main sequence routine */
seq_t a(idx_t n) {
    /* n ≥ 1 by OEIS convention; behaviour for n == 0 is undefined */
    if (n == 0) return (seq_t)0;

    /* Need at least the (n+1)-th composite */
    ensure_limit((size_t)n + 1);

    idx_t c_n   = composites[n - 1];     /* n-th composite (1‑based) */
    idx_t c_n1  = composites[n];        /* (n+1)-th composite */
    idx_t count = prime_prefix[c_n1 - 1] - prime_prefix[c_n]; /* primes in (c_n, c_n1) */

    return (seq_t)count;
}
