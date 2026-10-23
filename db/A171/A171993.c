/*
 * A171993  Numbers that are neither prime nor multiples of 3.
 * https://oeis.org/A171993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   is_prime, seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A171993: numbers that are neither prime
   nor multiples of 3. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* static cache of already computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure that the cache contains at least n terms
   (n >= 1).  The function expands the cache if needed. */
static void ensure_cache(idx_t n)
{
    if (cache_size >= n)
        return;

    /* allocate a new array that can hold n terms */
    seq_t *new_cache = seq_alloc_array((size_t)n);
    if (!new_cache)
        return;          /* out of memory – should not happen */

    /* copy previously computed terms */
    for (idx_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    /* compute additional terms */
    idx_t count = cache_size;
    uint64_t cand;

    if (cache_size == 0) {
        /* first term is 1 */
        new_cache[0] = (seq_t)1;
        count = 1;
        cand = 2;
    } else {
        /* start scanning after the last stored value */
        cand = (uint64_t)new_cache[cache_size - 1] + 1;
    }

    while (count < n) {
        if (cand % 3 != 0 && !is_prime(cand)) {
            new_cache[count] = (seq_t)cand;
            ++count;
        }
        ++cand;
    }

    /* free the old cache and replace it */
    if (cache)
        seq_free_array(cache);
    cache     = new_cache;
    cache_size = n;
}

/* Return the nth element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but safe default */

    ensure_cache(n);
    return cache[n - 1];
}
