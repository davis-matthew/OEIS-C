/*
 * A039094  Numbers whose base-9 representation has the same number of 2's and
 *          6's.
 * https://oeis.org/A039094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, equal_2_6, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039094: numbers whose base‑9 representation has the same number of 2's and 6's */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/equal_2_6.h"

/* Static cache of found terms. */
static seq_t *cache      = NULL;
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* allocated capacity of the array */
static idx_t  next_candidate = 0;    /* next integer to test */

/* Grow the cache array to at least new_cap elements.
   Allocation and deallocation are done with the provided helpers so
   the code works for arbitrary‑precision seq_t types. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

    /* Allocate a new array. */
    seq_t *tmp = alloc_seq_array(new_cap);
    if (!tmp) {
        /* Allocation failed – let the program abort. */
        abort();
    }

    /* Copy existing elements element‑wise (memcpy is unsafe for non‑POJ types). */
    for (size_t i = 0; i < cache_size; ++i)
        tmp[i] = cache[i];

    /* Free old array and update pointers. */
    if (cache)
        free_seq_array(cache);
    cache = tmp;
    cache_cap = new_cap;
}

/* Ensure the cache can hold at least n terms. */
static void ensure_cache_n(idx_t n)
{
    if (n <= (idx_t)cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1024;
    while (new_cap < (size_t)n)
        new_cap *= 2;
    grow_cache(new_cap);
}

/* Return the nth term of OEIS A039094 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;   /* undefined for n < 1 – return 0 */

    ensure_cache_n(n);

    /* Generate new terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        if (equal_2_6((seq_t)next_candidate))
            cache[cache_size++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
