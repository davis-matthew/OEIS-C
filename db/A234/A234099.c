/*
 * A234099  Integers of the form (p*q*r - 1)/2, where p, q, r are distinct
 *          primes.
 * https://oeis.org/A234099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of (p*q*r - 1)/2 for distinct primes p,q,r > 2. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"

/* Comparison function for qsort */
static int seq_cmp(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Maximum prime to generate.  A bound of 1000 guarantees
   more than 740,000 terms, comfortably exceeding n <= 10 000. */
#define PRIME_LIMIT 1000

seq_t a(idx_t n)
{
    /* n is 1-based; return 0 for the unused 0 index. */
    if (n == 0) return 0;

    /* Static cache to avoid recomputing values. */
    static seq_t *cache = NULL;   /* sorted values */
    static size_t cache_len = 0;  /* number of cached values */
    if (cache != NULL)            /* cache already built */
        return cache[n - 1];

    /* ------------------ Build cache ------------------ */
    /* Generate all primes > 2 up to PRIME_LIMIT. */
    uint64_t primes[PRIME_LIMIT / 2 + 1];  /* a small upper bound array */
    size_t primes_cnt = 0;
    for (uint64_t cand = 3; cand <= PRIME_LIMIT; cand += 2) {
        if (is_prime_small((seq_t)cand)) {
            primes[primes_cnt++] = cand;
        }
    }

    /* Total number of distinct triples */
    const size_t total = primes_cnt * (primes_cnt - 1) * (primes_cnt - 2) / 6;
    cache = (seq_t *)malloc(total * sizeof(seq_t));
    if (!cache) return 0;  /* allocation failure */

    /* Compute (p*q*r - 1)/2 for all distinct triples. */
    size_t idx = 0;
    for (size_t i = 0; i + 2 < primes_cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        for (size_t j = i + 1; j + 1 < primes_cnt; ++j) {
            seq_t q = (seq_t)primes[j];
            for (size_t l = j + 1; l < primes_cnt; ++l) {
                seq_t r = (seq_t)primes[l];
                seq_t prod = p * q * r;
                cache[idx++] = (prod - 1) / 2;
            }
        }
    }

    /* Sort so that a(1) <= a(2) <= ... */
    qsort(cache, total, sizeof(seq_t), seq_cmp);
    cache_len = total;

    /* Return the requested term. */
    return cache[n - 1];
}
