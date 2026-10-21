/*
 * A232950  Number of n X 3 0..3 arrays with no element x(i,j) adjacent to
 *          value 3-x(i,j) horizontally, diagonally or antidiagonally, and top
 *          left element zero.
 * https://oeis.org/A232950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 6*a(n-2), with a(1)=9, a(2)=129
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function caches the sequence values to allow arbitrary‑order calls. */

#include <stdlib.h>
#include <stddef.h>

/* Expand the cache array so it can hold indices 0..n (a_cache[0] is unused). */
static void ensure_capacity(idx_t n, seq_t **cache, size_t *cap)
{
    size_t needed = (size_t)(n + 1);
    if (needed <= *cap)
        return;
    seq_t *p = (seq_t *)realloc(*cache, needed * sizeof(seq_t));
    if (!p)
        abort();            /* allocation failure */
    *cache = p;
    *cap   = needed;
}

/* Cache for computed terms (indices 1..). */
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;
static idx_t a_cache_highest = 0;
static int a_cache_initialised = 0;

seq_t a(idx_t n)
{
    /* Initialise base cases once. */
    if (!a_cache_initialised) {
        /* Allocate enough space for indices 0..2. */
        ensure_capacity(2, &a_cache, &a_cache_cap);
        a_cache[1] = (seq_t)9;
        a_cache[2] = (seq_t)129;
        a_cache_highest = 2;
        a_cache_initialised = 1;
    }

    /* Return from cache if already computed. */
    if (n <= a_cache_highest)
        return a_cache[n];

    /* Ensure the cache is large enough. */
    ensure_capacity(n, &a_cache, &a_cache_cap);

    /* Compute missing terms using the recurrence. */
    for (idx_t i = a_cache_highest + 1; i <= n; ++i) {
        a_cache[i] = (seq_t)15 * a_cache[i-1] - (seq_t)6 * a_cache[i-2];
    }
    a_cache_highest = n;
    return a_cache[n];
}
