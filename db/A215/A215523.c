/*
 * A215523  Slowest increasing sequence of alternate-parity integers m such
 *          that 2m+1 is prime.
 * https://oeis.org/A215523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: A215523 is defined by starting with m(1)=1 and then, for
 * each n>1, choosing the smallest integer m>m(n-1) of opposite parity
 * such that 2m+1 is prime.  This implementation builds the sequence
 * incrementally, caching previously computed values to support arbitrary
 * call order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* 1‑based cache for computed terms */
static size_t cache_cap = 0;         /* allocated capacity of the cache   */
static idx_t  cache_len = 0;         /* largest index already computed */

/* Ensure the cache has room for at least 'needed' entries (index 1‑based). */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for index 0 (offset 1) */

    /* Return cached value if already known. */
    if (n <= cache_len)
        return cache[n];

    /* Extend the cache as far as needed. */
    grow_cache((size_t)n + 1);               /* allow 1‑based indexing */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t m;
        if (i == 1) {
            m = (seq_t)1;                    /* a(1) = 1 */
        } else {
            seq_t last = cache[cache_len];
            seq_t cand = last + (seq_t)1;    /* start at next integer */

            /* Ensure the parity alternates. */
            if ((cand & (seq_t)1) == (last & (seq_t)1))
                cand += (seq_t)1;

            /* Find the next m of the desired parity such that 2m+1 is prime. */
            for (;;) {
                seq_t p = cand + cand + (seq_t)1;   /* 2*cand + 1, no overflow in addition */
                if (is_prime_big(p)) {
                    m = cand;
                    break;
                }
                cand += (seq_t)2;                  /* keep parity */
            }
        }
        cache[i] = m;
        cache_len = i;
    }
    return cache[n];
}
