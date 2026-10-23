/*
 * A137632  Sums of 2 cubes of distinct odd primes.
 * https://oeis.org/A137632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137632: sums of two distinct odd prime cubes.
   The n‑th term is the n‑th smallest value of p^3 + q^3 where
   p and q are distinct odd primes.  The function generates all
   such sums up to a prime bound sufficient to provide at least
   n values, sorts them, removes duplicates, and caches the result
   for subsequent calls. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* Comparator for qsort on seq_t values */
static int seq_cmp(const void *x, const void *y)
{
    const seq_t *a = (const seq_t *)x;
    const seq_t *b = (const seq_t *)y;
    if (*a < *b) return -1;
    return (*a > *b) ? 1 : 0;
}

/* Generate all distinct sums of two cubes of odd primes up to 'limit'.
   The sorted unique values are returned in '*out' and '*len'.
   The caller must free '*out' when done. */
static void generate_cache(idx_t limit, seq_t **out, size_t *len)
{
    uint64_t *p_uint64 = NULL;
    size_t prime_cnt = 0;

    /* Get all primes <= limit */
    build_primes_upto(limit, &p_uint64, &prime_cnt);

    /* Skip prime 2 if present */
    size_t odd_start = 0;
    if (prime_cnt > 0 && p_uint64[0] == 2u) odd_start = 1;

    size_t odd_cnt = 0;
    if (prime_cnt > odd_start) odd_cnt = prime_cnt - odd_start;

    size_t pair_cnt = 0;
    if (odd_cnt >= 2) pair_cnt = odd_cnt * (odd_cnt - 1) / 2;

    if (pair_cnt == 0) {
        *out = NULL;
        *len = 0;
        free(p_uint64);
        return;
    }

    /* Allocate array for all pair sums */
    seq_t *pairs = (seq_t *)malloc(pair_cnt * sizeof(seq_t));
    if (!pairs) {
        *out = NULL;
        *len = 0;
        free(p_uint64);
        return;
    }

    /* Compute sums */
    size_t idx = 0;
    for (size_t i = odd_start; i < prime_cnt; ++i) {
        seq_t pi = (seq_t)p_uint64[i];
        seq_t pi3 = pi * pi * pi;
        for (size_t j = i + 1; j < prime_cnt; ++j) {
            seq_t pj = (seq_t)p_uint64[j];
            seq_t pj3 = pj * pj * pj;
            pairs[idx++] = pi3 + pj3;
        }
    }

    /* Sort sums */
    qsort(pairs, pair_cnt, sizeof(seq_t), seq_cmp);

    /* Deduplicate while preserving order */
    size_t uniq_len = 1;
    seq_t *uniq = (seq_t *)malloc(pair_cnt * sizeof(seq_t));
    if (!uniq) {
        free(pairs);
        free(p_uint64);
        *out = NULL;
        *len = 0;
        return;
    }
    uniq[0] = pairs[0];
    for (size_t k = 1; k < pair_cnt; ++k) {
        if (pairs[k] != pairs[k - 1]) {
            uniq[uniq_len++] = pairs[k];
        }
    }

    /* Clean up */
    free(pairs);
    free(p_uint64);

    *out = uniq;
    *len = uniq_len;
}

/* Cached data across calls */
static seq_t *cache = NULL;          /* sorted unique sums */
static size_t cache_len = 0;         /* number of terms in cache */
static idx_t prime_limit = 0;        /* current upper bound for primes */

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0 */

    /* Return cached value if available */
    if (cache && (size_t)(n - 1) < cache_len) {
        return cache[n - 1];
    }

    /* Initialize prime bound if needed */
    if (prime_limit == 0) prime_limit = 100;

    /* Expand cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        /* Generate new cache */
        seq_t *new_cache = NULL;
        size_t new_len = 0;
        generate_cache(prime_limit, &new_cache, &new_len);

        /* Replace old cache */
        free(cache);
        cache = new_cache;
        cache_len = new_len;

        /* If still insufficient, increase the prime bound */
        if (cache_len < (size_t)n) {
            prime_limit *= 2;
        }
    }

    return cache[n - 1];
}
