/*
 * A041839  Denominators of continued fraction convergents to sqrt(440).
 * https://oeis.org/A041839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=41, a(3)=42,
   a(n)=42*a(n-2)-a(n-4) for n ≥ 4. */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of computed terms */

    /* If the value is already cached, return it. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Allocate or grow the cache to hold up to index n. */
    if (cache_len == 0) {
        /* Ensure at least four slots for the base values. */
        size_t new_cap = (n + 1 < 4) ? 4 : (size_t)n + 1;
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – abort to satisfy strict requirements. */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1) {
            val = (seq_t)1;
        } else if (i == 2) {
            val = (seq_t)41;
        } else if (i == 3) {
            val = (seq_t)42;
        } else {
            /* a(i) = 42*a(i-2) - a(i-4) */
            val = (seq_t)42 * cache[i - 2] - cache[i - 4];
        }
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
