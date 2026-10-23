/*
 * A117572  Expansion of (1 + 2*x^2)/((1 - x^2)*(1 - x^3)).
 * https://oeis.org/A117572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A117572: Expansion of (1 + 2*x^2) / ((1 - x^2)*(1 - x^3))
 *  Recurrence: a(n) = a(n-2) + a(n-3) - a(n‑5)
 *  Initial values: a(0)=1, a(1)=0, a(2)=3, a(3)=1, a(4)=3
 *
 *  The function builds a static cache that grows to cover
 *  any requested n.  It uses the utility allocator
 *  "alloc_seq_array" so that seq_t may be an arbitrary‑precision
 *  integer class in a C++ build.  The cache is expanded only
 *  when a larger n is requested.
 */

#include <stdbool.h>
#include <string.h>
#include "../utils/alloc_seq_array.h"   /* provides seq_t *alloc_seq_array(size_t) */
#include "../utils/free_seq_array.h"    /* provides void free_seq_array(seq_t*) */

static seq_t *cache = NULL;          /* pointer to cached values   */
static idx_t  cache_size = 0;        /* allocated size (number of elements)   */
static idx_t  cache_done = 0;        /* highest index computed so far          */

/* Helper to initialise the first five entries */
static void initialise_base(void)
{
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)3;
    cache[3] = (seq_t)1;
    cache[4] = (seq_t)3;
    cache_done = 4;
}

/* Ensure that the cache holds at least (*n)+1 entries. */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        /* First allocation: at least 5 elements for the base case */
        cache_size = (n + 1 > 5) ? (n + 1) : 5;
        cache = alloc_seq_array(cache_size);
        initialise_base();
        return;
    }

    if (n < cache_size)
        return; /* no resize needed */

    /* Resize: allocate a new array, copy old data, free old array */
    idx_t new_size = n + 1;
    seq_t *new_cache = alloc_seq_array(new_size);
    size_t bytes = cache_done + 1;
    memcpy(new_cache, cache, bytes * sizeof(seq_t));

    free_seq_array(cache);
    cache = new_cache;
    cache_size = new_size;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* First, make sure we have space for n */
    ensure_capacity(n);

    /* If n is within the initially cached region, just return it */
    if (n <= cache_done)
        return cache[n];

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = cache_done + 1; i <= n; ++i) {
        /* a(i) = a(i-2) + a(i-3) - a(i-5) */
        cache[i] = cache[i - 2] + cache[i - 3] - cache[i - 5];
    }
    cache_done = n;

    return cache[n];
}
