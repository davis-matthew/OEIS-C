/*
 * A167443  Prime numbers ending in the prime number 41.
 * https://oeis.org/A167443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 157, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 882 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167443: nth prime ending in 41 (primes ≡ 41 (mod 100)) */
/* The function searches successive numbers of the form 100k+41, tests
   primality with is_prime(idx_t), and memoizes the results. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    static seq_t *cache = NULL;      /* cached primes */
    static size_t cache_size = 0;    /* number of cached primes */
    static size_t cache_cap  = 0;    /* capacity of cache array */
    static idx_t  candidate  = 41;   /* next number of the form 100k+41 */

    /* Convert 1-based index to 0-based target */
    size_t target = (size_t)n - 1;

    /* Initialise cache if needed */
    if (!cache) {
        cache_cap = 64;                     /* initial capacity */
        cache     = seq_alloc(cache_cap);   /* allocate array of seq_t */
    }

    /* Generate primes until we have at least (target+1) primes */
    while (cache_size <= target) {
        if (is_prime(candidate)) {
            /* Grow cache if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
                cache = new_cache;
                cache_cap = new_cap;
            }
            /* Store the prime (casting is safe for values that fit in seq_t) */
            cache[cache_size++] = (seq_t)candidate;
        }
        candidate += 100;    /* move to next number ending with 41 */
    }

    return cache[target];
}
