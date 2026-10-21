/*
 * A229439  Number of n X 2 0..2 arrays with horizontal differences mod 3
 *          never 1, vertical differences mod 3 never -1, and rows and columns
 *          lexicographically nondecreasing.
 * https://oeis.org/A229439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) for
 *            n>5, with a(1)=4, a(2)=7, a(3)=13, a(4)=25, a(5)=47
 * Range:     u8 n <= 8, u16 n <= 34, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses linear recurrence, so evaluation is O(n). */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"   /* declares seq_alloc_new_array */

static seq_t *cache   = NULL;   /* memoisation array, indices start at 1 */
static idx_t cache_sz = 0;      /* allocated size (length of cache) */

/* Ensure that cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if (n < 1) return;

    /* We need space for indices 0..max(5,n).  Index 0 is unused. */
    idx_t need   = (n > (idx_t)5) ? n : (idx_t)5;
    size_t new_sz = (size_t)need + 1;     /* include slot 0 */

    if (new_sz <= (size_t)cache_sz) return; /* already large enough */

    /* Allocate new array. */
    seq_t *new_cache = seq_alloc_new_array(new_sz);
    if (!new_cache) exit(EXIT_FAILURE); /* allocation failure */

    /* Copy existing cached values. */
    for (idx_t i = 0; i < cache_sz; ++i) new_cache[i] = cache[i];

    /* Initialise base values if this is the first allocation. */
    if (cache_sz == 0) {
        new_cache[1] = (seq_t)4;
        new_cache[2] = (seq_t)7;
        new_cache[3] = (seq_t)13;
        new_cache[4] = (seq_t)25;
        new_cache[5] = (seq_t)47;
    }

    /* Compute missing values using the recurrence. */
    idx_t start = (cache_sz > (idx_t)5) ? cache_sz : (idx_t)6;
    for (idx_t i = start; i <= need; ++i) {
        new_cache[i] =
            (seq_t)5 * new_cache[i-1]                       /* +5*a(i-1) */
            - (seq_t)10 * new_cache[i-2]                    /* -10*a(i-2) */
            + (seq_t)10 * new_cache[i-3]                    /* +10*a(i-3) */
            - (seq_t)5  * new_cache[i-4]                    /* -5*a(i-4) */
            +        new_cache[i-5];                       /* +   a(i-5) */
    }

    /* Replace old cache. */
    if (cache) free(cache);
    cache    = new_cache;
    cache_sz = new_sz;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n];
}
