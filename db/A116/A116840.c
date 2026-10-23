/*
 * A116840  Number of permutations of length n which avoid the patterns 1234,
 *          1243, 2431.
 * https://oeis.org/A116840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10·a(n‑1) – 42·a(n‑2) + 97·a(n‑3) – 135·a(n‑4) +
 *            116·a(n‑5) – 60·a(n‑6) + 17·a(n‑7) – 2·a(n‑8) (for n ≥ 10)
 *            Initial values: a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=75,
 *            a(6)=256, a(7)=826, a(8)=2535, a(9)=7474
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__SIZEOF_INT128__)
#  define USE_INT128 1
#else
#  define USE_INT128 0
#endif

/* Static cache for computed terms.  cache[1] … cache[max_computed] */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t   cache_max = 0;

/* Ensure the cache can store values up to and including n.  */
static void ensure_cache(idx_t n)
{
    if (!cache) {
        size_t cap = (size_t)n + 1;
        if (cap < 10) cap = 10;               /* we need first 9 terms */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return;                   /* allocation failed – undefined */
        for (size_t i = 0; i < cap; ++i) cache[i] = 0;
        /* initialise the first 9 terms (values that fit in seq_t are correct) */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)75;
        cache[6] = (seq_t)256;
        cache[7] = (seq_t)826;
        cache[8] = (seq_t)2535;
        cache[9] = (seq_t)7474;
        cache_max = 9;
        cache_cap = cap;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 10) new_cap = 10;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;                     /* allocation failed – undefined */
        cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* undefined for n < 1 */

    ensure_cache(n);

    if (n <= cache_max) return cache[n];

    for (idx_t i = cache_max + 1; i <= n; ++i) {
#if USE_INT128
        unsigned __int128 tmp = (unsigned __int128)cache[i-1] * 10;
        tmp -= (unsigned __int128)cache[i-2] * 42;
        tmp += (unsigned __int128)cache[i-3] * 97;
        tmp -= (unsigned __int128)cache[i-4] * 135;
        tmp += (unsigned __int128)cache[i-5] * 116;
        tmp -= (unsigned __int128)cache[i-6] * 60;
        tmp += (unsigned __int128)cache[i-7] * 17;
        tmp -= (unsigned __int128)cache[i-8] * 2;
        cache[i] = (seq_t)tmp;
#else
        seq_t tmp = cache[i-1] * 10;
        tmp -= cache[i-2] * 42;
        tmp += cache[i-3] * 97;
        tmp -= cache[i-4] * 135;
        tmp += cache[i-5] * 116;
        tmp -= cache[i-6] * 60;
        tmp += cache[i-7] * 17;
        tmp -= cache[i-8] * 2;
        cache[i] = tmp;
#endif
    }
    cache_max = n;
    return cache[n];
}
