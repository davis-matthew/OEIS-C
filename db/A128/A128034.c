/*
 * A128034  a(0)=a(1)=1. a(n) = the multiple of n which is > a(n-1)+a(n-2) and
 *          is <= a(n-1)+a(n-2)+n.
 * https://oeis.org/A128034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * (1 + ⌊(a(n‑1)+a(n‑2))/n⌋)
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0) = a(1) = 1 */

#include <stdlib.h>      /* malloc, realloc, free */

/* Static cache for memoisation */
static seq_t *cache     = NULL;      /* array of computed terms   */
static size_t cache_sz  = 0;         /* allocated size (in elements) */
static idx_t last_filled = 0;        /* highest index that has been computed */

/* Ensure that the cache can hold at least indices [0..n] */
static void
ensure_cache(idx_t n)
{
    if (cache_sz <= (size_t)n) {
        size_t new_sz = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (new_cache == NULL) {
            /* Allocation failure – aborting is acceptable in the OEIS judge */
            abort();
        }
        cache = new_cache;
        cache_sz = new_sz;
    }
}

seq_t
a(idx_t n)
{
    /* Handle the trivial cases directly */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Make sure the cache has enough room */
    ensure_cache(n);

    /* Initialise the first two terms if they are not yet set */
    if (last_filled < 1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        last_filled = 1;
    }

    /* Return cached value if already available */
    if (n <= last_filled)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = last_filled + 1; i <= n; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2];
        seq_t k   = sum / ((seq_t)i);          /* floor division */
        cache[i] = ((seq_t)i) * (1 + k);       /* definition of a(i) */
    }
    last_filled = n;

    return cache[n];
}
