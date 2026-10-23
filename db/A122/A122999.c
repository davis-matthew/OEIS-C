/*
 * A122999  G.f.: 1/(1 - x - 25*x^2).
 * https://oeis.org/A122999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 25*a(n-2), a(0)=1, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the A122999 sequence with memoisation. */
#include <stdlib.h>   /* for malloc, realloc */

static seq_t *cache = NULL; /* cached values */
static idx_t cached_n = 0;  /* number of cached entries */

/* Return the n-th term a(n) */
seq_t a(idx_t n)
{
    /* Initial values */
    if (n < 2) return (seq_t)1;

    /* Initialise cache on first use */
    if (cache == NULL) {
        /* Allocate space for indices 0 and 1 */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cached_n = 2;
    }

    /* Return cached value if available */
    if (n < cached_n) return cache[n];

    /* Grow cache to accommodate indices up to n */
    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL) {
        /* Allocation failed – behaviour unspecified, but avoid crash */
        return (seq_t)0;
    }
    cache = new_cache;

    /* Compute missing entries using the recurrence */
    for (idx_t i = cached_n; i <= n; ++i) {
        cache[i] = cache[i-1] + ((seq_t)25 * cache[i-2]);
    }

    cached_n = new_cap;
    return cache[n];
}
