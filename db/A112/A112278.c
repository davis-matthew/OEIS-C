/*
 * A112278  a(0) = 1, a(1) = 2; thereafter a(n) = prime(mod(a(n-1),100))+1.
 * https://oeis.org/A112278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less, changed
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 58 (2026-11-03)
 * Helpers:   is_prime_trial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 2; thereafter a(n) = prime(mod(a(n-1),100)) + 1. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_trial.h"
#include "../utils/mem.h"

/*--- dynamic array helpers for primes --------------------------------------*/
static void ensure_primes_cap(size_t *cap, uint64_t **arr, size_t min_cap) {
    if (*cap < min_cap) {
        size_t new_cap = (*cap) ? (*cap) << 1 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t get_prime(uint64_t idx) {
    static uint64_t *prime_vec = NULL;
    static size_t prime_cap = 0;
    static size_t prime_sz  = 0;
    static uint64_t candidate = 3;      /* next odd to test */

    if (!prime_vec) { /* first call: initialise */
        ensure_primes_cap(&prime_cap, &prime_vec, 16);
        prime_vec[0] = 2;
        prime_sz = 1;
    }

    while (prime_sz < idx) {
        /* find next prime */
        while (!is_prime_trial(candidate, prime_vec, prime_sz))
            candidate += 2;

        /* add found prime */
        if (prime_sz == prime_cap)
            ensure_primes_cap(&prime_cap, &prime_vec, prime_cap << 1);

        prime_vec[prime_sz++] = candidate;
        candidate += 2;
    }
    return (seq_t)prime_vec[idx - 1];
}

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap   = 0;

seq_t a(idx_t n)
{
    if (cache == NULL) {
        cache_cap   = 2;
        cache       = seq_malloc(cache_cap);
        cache[0]    = (seq_t)1;
        cache[1]    = (seq_t)2;
        cache_size  = 2;
    }
    if (n < cache_size)
        return cache[n];

    /* Ensure the cache is large enough */
    if (n + 1 > cache_cap) {
        cache_cap   = n + 1;
        cache       = seq_realloc(cache, cache_cap);
    }

    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        uint64_t idx_prime = (uint64_t)(prev % (seq_t)100);
        if (idx_prime == 0) {
            cache[i] = (seq_t)1;       /* Defend against an unlikely 0 index. */
        } else {
            seq_t p = get_prime(idx_prime);
            cache[i] = p + (seq_t)1;
        }
    }
    cache_size = n + 1;
    return cache[n];
}
