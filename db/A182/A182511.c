/*
 * A182511  a(0)=1, a(1)=2, a(n) = (a(n-2)*a(n-1) mod n) + 1.
 * https://oeis.org/A182511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 2, a(n) = (a(n-2) * a(n-1) mod n) + 1 (n >= 2)
 * Range:     u8 n <= 72, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a static cache that grows as needed.
 * It works for any seq_t size (unsigned 8/16/32/64/128 bits or arbitrary precision),
 * handling the product modulo n without overflow by choosing the
 * appropriate intermediate type. */

#include <stdlib.h>
#include <stdint.h>

/* Static cache data */
static seq_t *cache = NULL;     /* array of computed values   */
static size_t cache_cap = 0;    /* allocated capacity         */
static size_t computed_max = 1; /* highest index that has been computed */

/* Ensure the cache has at least n+1 slots. */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort(); /* memory allocation failed */
    /* Initialize new slots to zero */
    for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Initialize cache on first call */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        computed_max = 1;
    }

    /* Handle base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* If already computed, return the value */
    if (n <= computed_max) return cache[n];

    /* Ensure sufficient capacity in the cache */
    ensure_capacity(n);

    /* Compute missing terms up to n */
    for (size_t k = computed_max + 1; k <= (size_t)n; ++k) {
        seq_t prev2 = cache[k - 2];
        seq_t prev1 = cache[k - 1];
        seq_t modk  = (seq_t)k;
        seq_t prod_mod;

        /* Choose intermediate type based on seq_t size to avoid overflow */
        if (sizeof(seq_t) <= 4) {
            /* seq_t is 8/16/32 bits: use 64-bit arithmetic */
            unsigned long long a_mod = (unsigned long long)(prev2 % modk);
            unsigned long long b_mod = (unsigned long long)(prev1 % modk);
            unsigned long long prod = a_mod * b_mod;
            prod_mod = (seq_t)(prod % (unsigned long long)modk);
        } else {
            /* seq_t is 64 bits or larger: use 128-bit arithmetic */
#if defined(__SIZEOF_INT128__) || defined(_MSC_VER)
            unsigned __int128 a_mod = (unsigned __int128)(prev2 % modk);
            unsigned __int128 b_mod = (unsigned __int128)(prev1 % modk);
            unsigned __int128 prod = a_mod * b_mod;
            prod_mod = (seq_t)(prod % (unsigned __int128)modk);
#else
            /* Fallback: use multiprecision style (may overflow for very large seq_t) */
            unsigned long long a_mod = (unsigned long long)(prev2 % modk);
            unsigned long long b_mod = (unsigned long long)(prev1 % modk);
            unsigned long long prod = a_mod * b_mod;
            prod_mod = (seq_t)(prod % (unsigned long long)modk);
#endif
        }

        cache[k] = prod_mod + (seq_t)1;
    }
    computed_max = n;
    return cache[n];
}
