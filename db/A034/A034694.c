/*
 * A034694  Smallest prime == 1 (mod n).
 * https://oeis.org/A034694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = the smallest prime p with p ≡ 1 (mod n), i.e. p = k*n + 1
 *            with k > 0 and p prime
 * Range:     u8 n <= 30, u16 n <= 1690, u32 n <= 9061, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"   /* provides is_prime_big(seq_t) */

static seq_t *cache = NULL;      /* cache[n] = a(n); cache[0] unused */
static size_t cache_size = 0;    /* allocated cache size (>= max n + 1) */

/* Ensure that cache can hold indices up to n. */
static void ensure_cache_size(idx_t n)
{
    /* In OEIS indexing, n ≥ 1. n+1 guarantees space for index n. */
    if ((size_t)n + 1 <= cache_size)
        return;

    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = NULL;

#ifdef __cplusplus
    /* C++: allocate a new array and copy old values. */
    new_cache = new seq_t[new_size];
    if (cache) {
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
    }
#else
    /* C: use realloc and initialize new elements to 0. */
    new_cache = (seq_t *)realloc(cache, new_size * sizeof(*new_cache));
    if (!new_cache) {
        /* allocation failure – abort */
        exit(EXIT_FAILURE);
    }
    for (size_t i = cache_size; i < new_size; ++i)
        new_cache[i] = 0;
#endif

    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for index 0, but protect against it */

    ensure_cache_size(n);

    if (cache[n] != 0)
        return cache[n];

    seq_t N = (seq_t)n;
    idx_t k = 1;
    while (1) {
        seq_t p = N * (seq_t)k + 1;   /* candidate prime */
        if (is_prime_big(p)) {
            cache[n] = p;
            return p;
        }
        ++k;
    }
}
