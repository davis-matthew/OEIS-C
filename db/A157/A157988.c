/*
 * A157988  a(n)=16*n!/(253*43!).
 * https://oeis.org/A157988
 *
 * Offset:    46    (a(46) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 46, u32 n <= 49, u64 n <= 55, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) * n   (n > 46),  a(46) = 5760 */

#include <stdlib.h>   /* realloc, exit */

/* Starting index of the sequence in OEIS notation. */
static const idx_t OFFSET = 46ULL;

/* Cache of previously computed values.  Elements correspond to indices
   OFFSET .. n, so cache[i] holds a(OFFSET + i). */
static seq_t *cache     = NULL;   /* dynamic array of seq_t */
static size_t cache_len = 0;      /* number of cached elements */

/* Ensure that cache contains values up to and including index n. */
static void ensure_cache(idx_t n)
{
    if (n < OFFSET)          /* nothing to cache for indices < 46 */
        return;

    /* How many elements are required?  Elements for indices
       OFFSET .. n inclusive. */
    size_t needed = (size_t)(n - OFFSET + 1);
    if (needed <= cache_len)
        return;      /* already cached */

    /* Reallocate cache to the larger size. */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * needed);
    if (!tmp) {
        /* Allocation failed – abort the program.  This is safe for
           both the C and C++ compilation modes. */
        exit(1);
    }

    /* Initialise newly added elements. */
    if (cache_len == 0) {
        /* Base case a(46) */
        tmp[0] = (seq_t)5760;
        cache_len = 1;
    }

    /* Compute a(n) from a(n-1) * n for all missing indices. */
    for (idx_t i = (idx_t)cache_len; i < (idx_t)needed; ++i) {
        idx_t curr = OFFSET + i;              /* current index */
        tmp[i] = tmp[i - 1] * (seq_t)curr;    /* a(curr) = a(curr-1) * curr */
    }

    cache     = tmp;
    cache_len = needed;
}

/* External function required by the OEIS framework. */
seq_t a(idx_t n)
{
    if (n < OFFSET)
        return 0;             /* sequence undefined for n < 46 */

    ensure_cache(n);
    return cache[n - OFFSET];
}
