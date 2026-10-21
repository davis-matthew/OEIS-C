/*
 * A097840  Chebyshev polynomials S(n,83) + S(n-1,83) with Diophantine
 *          property.
 * https://oeis.org/A097840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 83*a(n-1) - a(n-2) (a(0)=1, a(1)=84)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Dynamically expanding cache for computed terms */
static seq_t *cache = NULL;          /* cached values */
static idx_t cache_len = 0;          /* number of stored terms (cache_len-1 is max index cached) */
static size_t cache_cap = 0;         /* allocated capacity */

/* Ensure the cache can hold values up to index up_to, computing missing ones. */
static void ensure_cache(idx_t up_to)
{
    if (up_to < cache_len)
        return; /* already computed */

    /* Grow capacity if needed */
    if (up_to + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap <= (size_t)up_to + 1)
            new_cap *= 2;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) {
            /* In a real program we would handle out‑of‑memory properly */
            abort();
        }
        cache = new_ptr;
        cache_cap = new_cap;
    }

    /* Initialize first two terms if not yet done */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
        if (up_to >= 1) {
            cache[1] = (seq_t)84;
            cache_len = 2;
        }
    } else if (cache_len == 1 && up_to >= 1) {
        cache[1] = (seq_t)84;
        cache_len = 2;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= up_to; ++i) {
        seq_t mul = (seq_t)83 * cache[i - 1];
        cache[i] = mul - cache[i - 2];
    }
    cache_len = up_to + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
