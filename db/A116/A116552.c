/*
 * A116552  a(2*n+1) = 3*a(n), a(2*n+2) = 4*a(n) + a(n-1).
 * https://oeis.org/A116552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n+1) = 3*a(n), a(2*n+2) = 4*a(n)+a(n-1)
 * Range:     u8 n <= 21, u16 n <= 317, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A116552.  Memoised array ensures O(max(n)) cost over all
 * calls, independent of order. */
#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed values */
static idx_t  cap   = 0;      /* allocated capacity (in seq_t elements) */
static idx_t  size  = 0;      /* number of elements actually computed */

/* Allocate or grow the cache as necessary to hold up to idx indices */
static void ensure_cache(idx_t n)
{
    /* need space for indices 0 … n (i.e. n+1 elements) */
    if (cap > n) return;

    /* Grow: start with a small size if uninitialised */
    idx_t new_cap = cap ? cap << 1 : 4;
    while (new_cap <= n) new_cap <<= 1;   /* make sure new_cap > n */

    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);  /* abort on allocation failure */
    cache = new_cache;
    cap   = new_cap;

    /* Initialise new slots lazily during the filling loop below */
}

/* Main entry */
seq_t a(idx_t n)
{
    /* Handle very small indices directly (avoid underflow in recurrence) */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)4;

    /* Ensure we have a cache big enough for all indices we are about to
       compute.  If we already have enough computed values, simply return
       the cached answer. */
    if (size > n) return cache[n];

    /* Grow the cache if needed and start filling from the current size. */
    ensure_cache(n);
    if (size == 0) {                 /* first time: preset the base cases */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)4;
        size = 3;
    }

    /* Fill the cache up to index n inclusively. */
    for (idx_t i = size; i <= n; ++i) {
        if (i == 0) { cache[i] = (seq_t)1; }
        else if (i == 1) { cache[i] = (seq_t)3; }
        else if (i == 2) { cache[i] = (seq_t)4; }
        else if (i & 1U) {            /* odd: i = 2*k+1 */
            idx_t k = (i - 1) >> 1;
            cache[i] = (seq_t)3 * cache[k];
        } else {                      /* even (i>=4): i = 2*k+2 */
            idx_t k = (i - 2) >> 1;   /* k >= 1 here */
            cache[i] = (seq_t)4 * cache[k] + cache[k - 1];
        }
    }
    size = n + 1;  /* we have computed up to n */
    return cache[n];
}
