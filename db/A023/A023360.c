/*
 * A023360  Number of compositions of n into prime parts.
 * https://oeis.org/A023360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{prime p<=n} a(n-p) with a(0)=1
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 60, u64 n <= 116,
 *            u128 n <= 230, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

static seq_t *cache = NULL;          /* dynamic cache for a(n) */
static size_t cache_cap = 0;         /* capacity of cache array */
static idx_t computed_max = 0;       /* largest n that has been computed */

static uint64_t *prime_list = NULL;  /* array of primes */
static size_t primes_used = 0;       /* number of primes stored */

/* Ensure that prime_list contains all primes <= n.  Rebuild only if needed. */
static void ensure_primes_upto(idx_t n)
{
    static idx_t prime_limit = 0;    /* highest number for which primes were generated */
    if ((idx_t)n > prime_limit) {
        free(prime_list);            /* if there was a previous allocation */
        size_t count = 0;
        build_primes_upto((uint64_t)n, &prime_list, &count);
        primes_used  = count;
        prime_limit  = n;
    }
}

/* Grow cache so that cache[n] is valid. */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;            /* allocation failed – leave cache unchanged */
        /* initialize the newly added entries */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute a(n) using memoised dynamic programming. */
seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0) return (seq_t)1;

    /* Initialise cache at index 0 if not already done */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)1;
    }

    ensure_cache(n);

    /* If the maximum already computed is large enough, simply return the value. */
    if (computed_max >= n) return cache[n];

    /* Extend prime list if needed. */
    ensure_primes_upto(n);

    /* Compute values for all indices from computed_max + 1 up to n. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* Sum over all primes <= i */
        for (size_t k = 0; k < primes_used && prime_list[k] <= (uint64_t)i; ++k) {
            idx_t idx = i - (idx_t)prime_list[k];
            sum += cache[idx];
        }
        cache[i] = sum;
    }
    computed_max = n;

    return cache[n];
}
