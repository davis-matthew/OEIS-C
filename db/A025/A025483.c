/*
 * A025483  a(0) = 0; a(n) = a(n/4)/4 if n = a(n/4) = 0 (mod 4); a(n) =
 *          a(n-1)+1 otherwise.
 * https://oeis.org/A025483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; For n > 0, if n % 4 == 0 and a(n/4) % 4 == 0 a(n) =
 *            a(n/4) / 4 else a(n) = a(n-1) + 1 (The recurrence works for any
 *            order of calls and caches results.)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* Cache of computed values */
static size_t cache_size = 0;        /* Number of stored entries in cache */

/* Extend the cache to at least new_cap entries. */
static void extend_cache(size_t new_cap) {
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1); /* allocation failed */
    /* Initialize new slots to 0 (not strictly necessary for 0 case) */
    if (new_cap > cache_size) {
        for (size_t i = cache_size; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
    }
    cache = tmp;
    cache_size = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* Ensure cache capacity */
    if (n >= cache_size) {
        size_t new_cap = (cache_size == 0) ? 64 : cache_size * 2;
        while (new_cap <= n) new_cap *= 2;
        extend_cache(new_cap);
    }

    /* Return cached value if available */
    if (cache[n] != (seq_t)0) return cache[n];

    /* Recurse to compute a(n) */
    seq_t result;
    if ((n % (idx_t)4) == 0) {
        idx_t k = n / 4;
        seq_t a_k = a(k);
        if ((a_k % (seq_t)4) == 0) {
            result = a_k / (seq_t)4;
        } else {
            result = a(n - 1) + (seq_t)1;
        }
    } else {
        result = a(n - 1) + (seq_t)1;
    }

    cache[n] = result;
    return result;
}
