/*
 * A116533  a(1)=1, a(2)=2, for n > 2 if a(n-1) is prime, then a(n) =
 *          2*a(n-1), otherwise a(n) = a(n-1) - 1.
 * https://oeis.org/A116533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(2) = 2; for n > 2, a(n) = 2*a(n‑1) if a(n‑1) is
 *            prime, else a(n‑1)‑1
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;   /* 0‑based cache: cache[0] = a(1) */
static size_t cache_len = 0;  /* number of entries currently stored */
static size_t cache_cap = 0;  /* allocated capacity of cache */

/* Make sure cache has at least 'needed' slots. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* realloc failure leaves cache unchanged; caller will error on next use. */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)                           /* invalid index – behaviour undefined */
        return (seq_t)0;

    /* Initialise base cases once */
    if (cache_len == 0) {
        ensure_cache(2);
        cache[0] = (seq_t)1;              /* a(1) */
        cache[1] = (seq_t)2;              /* a(2) */
        cache_len = 2;
    }

    /* Return cached value if already available */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Compute needed terms */
    size_t start = cache_len;             /* start index (0‑based) to compute */
    ensure_cache((size_t)n);
    for (size_t i = start; i < (size_t)n; ++i) {
        seq_t prev = cache[i - 1];        /* a(i) because cache[0] = a(1) */
        if (is_prime_big(prev))
            cache[i] = prev * (seq_t)2;   /* a(i+1) = 2*a(i) */
        else
            cache[i] = prev - (seq_t)1;   /* a(i+1) = a(i)‑1   */
    }
    cache_len = (size_t)n;
    return cache[(size_t)n - 1];
}
