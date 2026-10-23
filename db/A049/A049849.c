/*
 * A049849  a(n) is the number of primes q less than n-th prime p for which
 *          the Euclidean algorithm for p,q has exactly 3 nonzero remainders.
 * https://oeis.org/A049849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   compute_prime_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function: computes OEIS sequence A049849.
   a(n) = number of primes q < p_n (the n-th prime) such that the Euclidean
   algorithm applied to (p_n, q) has exactly three non‑zero remainders. */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/compute_prime_limit.h"

static void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out) {
    /* Simple Sieve of Eratosthenes, suitable for limits up to a few 10⁶. */
    size_t lim = (size_t)limit;
    char *is_prime = (char *)calloc(lim + 1, 1);   /* all 0 initially */
    if (!is_prime) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    for (size_t i = 2; i <= lim; ++i) is_prime[i] = 1;
    for (size_t i = 2; i * i <= lim; ++i) {
        if (!is_prime[i]) continue;
        for (size_t j = i * i; j <= lim; j += i) is_prime[j] = 0;
    }
    size_t cnt = 0;
    for (size_t i = 2; i <= lim; ++i) if (is_prime[i]) ++cnt;
    uint64_t *list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!list) {
        free(is_prime);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }
    size_t idx = 0;
    for (size_t i = 2; i <= lim; ++i) if (is_prime[i]) list[idx++] = (uint64_t)i;
    free(is_prime);
    *primes_out = list;
    *cnt_out = cnt;
}

seq_t a(idx_t n) {
    /* Static cache for already computed a(k) values and for primes. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of computed terms */
    static size_t cache_cap = 0;   /* allocated size of cache */

    static uint64_t *prime_list = NULL;
    static size_t prime_count = 0; /* actual number of primes stored */
    static idx_t prime_limit = 0;  /* limit up to which primes were generated */

    if (n == 0) return 0; /* dummy for safety; not used in OEIS offset */

    /* Expand cache if necessary */
    if ((size_t)n > cache_len) {
        size_t old_len = cache_len;
        size_t new_len = (size_t)n;

        /* Ensure cache has enough capacity */
        if (cache_cap < new_len) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 64;
            while (new_cap < new_len) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failure – abort */ }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate primes up to the n-th prime if needed */
        idx_t required_limit = compute_prime_limit(n);
        if (!prime_list || required_limit > prime_limit) {
            free(prime_list);
            build_primes_upto(required_limit, &prime_list, &prime_count);
            prime_limit = required_limit;
        }

        /* Compute a(k) for k = old_len+1 ... n */
        for (size_t i = old_len; i < new_len; ++i) {
            uint64_t p = prime_list[i];
            seq_t cnt = 0;
            for (size_t j = 0; j < i; ++j) {
                uint64_t q = prime_list[j];
                uint64_t r2 = p % q;          /* always > 0 for distinct primes */
                uint64_t r3 = q % r2;         /* must be > 0 for next step */
                if (r3 == 0) continue;
                uint64_t r4 = r2 % r3;        /* must be > 0 for exactly 3 non‑zero remainders */
                if (r4 == 0) continue;
                if (r3 % r4 == 0) {
                    ++cnt;                  /* condition satisfied */
                }
            }
            cache[i] = cnt;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n - 1];
}
