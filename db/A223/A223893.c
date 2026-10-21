/*
 * A223893  Number of partitions of n into at most three distinct primes.
 * https://oeis.org/A223893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 382, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of partitions of n into at most three
   distinct primes.  The function counts 1‑, 2‑ and 3‑prime
   partitions by enumerating all primes ≤ n. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes.h"

/* Static cache of previously computed values.  cache[k] holds a(k). */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of stored entries */

/* Recompute the cache up to new_max (inclusive).  The function
   generates all primes ≤ new_max, then counts 1‑, 2‑ and 3‑prime
   partitions, storing the results in a new array that replaces
   the old cache. */
static void recompute_cache(idx_t new_max)
{
    /* Allocate a fresh cache of size new_max + 1 (index 0 unused). */
    size_t sz = (size_t)new_max + 1;
    seq_t *new_cache = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!new_cache) return;   /* allocation failure – leave cache unchanged */

    /* Generate all primes up to new_max.  build_primes returns an array
       of idx_t (the same type as n). */
    idx_t *primes = NULL;
    size_t pcnt = 0;
    build_primes(new_max, &primes, &pcnt);

    /* 1‑prime partitions – each prime itself. */
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t p = primes[i];
        if (p <= new_max) new_cache[(size_t)p] += 1;
    }

    /* 2‑prime partitions – unordered pairs of distinct primes. */
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t p1 = primes[i];
        if (p1 > new_max) break;
        for (size_t j = i + 1; j < pcnt; ++j) {
            idx_t p2 = primes[j];
            idx_t sum = p1 + p2;
            if (sum > new_max) break;
            new_cache[(size_t)sum] += 1;
        }
    }

    /* 3‑prime partitions – unordered triples of distinct primes. */
    for (size_t i = 0; i < pcnt; ++i) {
        idx_t p1 = primes[i];
        if (p1 > new_max) break;
        for (size_t j = i + 1; j < pcnt; ++j) {
            idx_t p2 = primes[j];
            if (p1 + p2 > new_max) break;
            for (size_t k = j + 1; k < pcnt; ++k) {
                idx_t p3 = primes[k];
                idx_t sum = p1 + p2 + p3;
                if (sum > new_max) break;
                new_cache[(size_t)sum] += 1;
            }
        }
    }

    /* Free the prime list and replace the old cache. */
    free(primes);
    free(cache);
    cache = new_cache;
    cache_size = sz;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset starts at 1 */
    if (cache == NULL || (size_t)n >= cache_size)
        recompute_cache(n);
    return cache[(size_t)n];
}
