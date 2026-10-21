/*
 * A050096  a(n) = floor(a(n-1)/3) if this is positive and not yet in the
 *          sequence, otherwise a(n) = 8*a(n-1).
 * https://oeis.org/A050096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 84, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1
   a(n) = floor(a(n‑1)/3)      if that value is > 0 and has not yet appeared
          else                 a(n) = 8 · a(n‑1)             for n ≥ 2       */
#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t (idx_t) */

#include "../utils/alloc_seq_array.h"   /* alloc_seq_array() */
#include "../utils/free_seq_array.h"    /* free_seq_array() */

/* Global cache of the sequence values */
static seq_t *seq_cache = NULL;
static size_t   cache_len = 0;   /* number of terms currently stored   */
static size_t   cache_cap = 0;   /* allocated capacity (number of terms) */

/* Ensure that the cache can hold at least 'needed' terms.
   If larger capacity is required, reallocate and copy existing values.
   The helper functions work with any seq_t implementation. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < needed)
        new_cap <<= 1;   /* double until big enough */

    /* Allocate new array */
    seq_t *new_arr = alloc_seq_array(new_cap);
    if (!new_arr)
        exit(1);          /* allocation failed */

    /* Copy old values into the new array */
    for (size_t i = 0; i < cache_len; ++i)
        new_arr[i] = seq_cache[i];

    /* Free the old array */
    if (seq_cache)
        free_seq_array(seq_cache);

    seq_cache = new_arr;
    cache_cap = new_cap;
}

/* Test whether value v already occurs in seq_cache[0 .. upto-1] */
static int already_seen(seq_t v, size_t upto)
{
    for (size_t i = 0; i < upto; ++i)
        if (seq_cache[i] == v)
            return 1;
    return 0;
}

/* Main function that returns the nth term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for offset 1, just guard */

    /* Initialise cache with first term if empty */
    if (cache_len == 0) {
        ensure_cache(64);          /* allocate some space */
        seq_cache[0] = (seq_t)1;   /* a(1) = 1 */
        cache_len  = 1;
    }

    /* Generate terms up to the requested index */
    while (cache_len < (size_t)n) {
        seq_t prev = seq_cache[cache_len - 1];
        seq_t cand = prev / (seq_t)3;    /* integer division truncates */

        seq_t next;
        if (cand > (seq_t)0 && !already_seen(cand, cache_len))
            next = cand;
        else
            next = prev * (seq_t)8;

        /* Grow cache if necessary */
        if (cache_len == cache_cap)
            ensure_cache(cache_cap << 1);   /* double capacity */

        seq_cache[cache_len] = next;
        ++cache_len;
    }

    return seq_cache[n - 1];   /* offset 1: a(1) is stored at index 0 */
}
