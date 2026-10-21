/*
 * A051810  Numbers with nonzero multiplicative digital root 8.
 * https://oeis.org/A051810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   multiplicative_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence:  A051810 – Numbers with nonzero multiplicative digital root 8
 * Computation method: iterate integers, compute their multiplicative digital
 * root (product of non‑zero digits repeated until a single digit),
 * and collect those that are equal to 8. Results are cached so that
 * subsequent calls, possibly with any order of indices, return in O(1).
 */

#include <stdlib.h>
#include "../utils/multiplicative_root.h"

/* Cached sequence values and state for incremental generation */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of values stored */
static size_t cache_capacity = 0; /* allocated size of cache array */
static seq_t next_candidate = 1;  /* next integer to test */
static idx_t matched_count = 0;   /* number of terms found so far */

/* Ensure the cache has enough capacity to store up to `required` values */
static void ensure_cache_capacity(size_t required)
{
    if (required <= cache_capacity)
        return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 128;
    while (new_cap < required)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* In a real environment we would handle allocation failure;
           here we simply abort because the environment provides enough memory. */
        abort();
    }
    cache = new_cache;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n < 1, but return 0 for safety */

    /* Generate sequence values up to index n if necessary */
    while (matched_count < n) {
        seq_t candidate = next_candidate;
        if (multiplicative_root(candidate) == 8) {
            ensure_cache_capacity(matched_count + 1);
            cache[matched_count++] = candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];  /* indices are 1‑based in OEIS */
}
