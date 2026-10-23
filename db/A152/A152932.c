/*
 * A152932  Number of sets (in the Hausdorff metric geometry) at each location
 *          between two sets defining a polygonal configuration consisting of
 *          three 6-gonal polygonal components chained with string components
 *          of length l as l varies.
 * https://oeis.org/A152932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) for n > 3 with initial
 *            values: a(1)=32733, a(2)=80361, a(3)=215658
 * Range:     u8 none, u16 n <= 1, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Memoisation cache (1‑based indexing) */
static seq_t *cache   = NULL;     /* pointer to cached terms */
static size_t cache_cap = 0;      /* allocated capacity (indexes 0..cap-1) */
static size_t cache_size = 0;     /* highest index currently stored (>=0) */

/* Initialise the cache with the first three terms if it is empty. */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    /* Allocate space for indices 0..3 (needs 4 elements). */
    size_t cap = 4;
    cache = (seq_t*)malloc(cap * sizeof(seq_t));
    if (cache == NULL)
        exit(1);                      /* memory allocation failed */
    cache_cap  = cap;
    cache_size = 3;                   /* we will store a(1), a(2), a(3) */
    cache[0] = 0;                     /* unused, for 1‑based alignment */
    cache[1] = (seq_t)32733;
    cache[2] = (seq_t)80361;
    cache[3] = (seq_t)215658;
}

/* Ensure capacity large enough to hold index n (0‑based). */
static void ensure_capacity(idx_t n)
{
    /* We need indices 0..n, inclusive. */
    if ((size_t)n < cache_cap)
        return;
    size_t new_cap = cache_cap;
    if (new_cap == 0)
        new_cap = 4;
    while (new_cap <= (size_t)n)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (new_ptr == NULL)
        exit(1);                      /* allocation failed */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Compute terms up to n if necessary and return a(n). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* value not defined for n<1, return 0 */

    init_cache();                    /* set up base terms */
    if (n <= (idx_t)cache_size)
        return cache[(size_t)n];

    /* Grow cache to accommodate up to n. */
    ensure_capacity(n);

    /* Compute missing terms using recurrence. */
    for (size_t i = cache_size + 1; i <= (size_t)n; ++i) {
        seq_t val = 2 * cache[i-1] + 2 * cache[i-2] - cache[i-3];
        cache[i] = val;
    }
    cache_size = (size_t)n;
    return cache[(size_t)n];
}
