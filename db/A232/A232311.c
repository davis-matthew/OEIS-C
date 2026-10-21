/*
 * A232311  Number of (n+1)X(3+1) 0..1 arrays with every element equal to some
 *          horizontal or antidiagonal neighbor, with top left element zero.
 * https://oeis.org/A232311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10*a(n-1) - 11*a(n-2) - 5*a(n-3) - a(n-4)
   initial values: a(1)=16, a(2)=139, a(3)=1202, a(4)=10409 */

/* The implementation uses a simple memoised recurrence.  The cache is
   grown with seq_realloc (from the OEIS utility library) so it works
   with arbitrary‑precision seq_t types.  Indexing is 1‑based; cache[0]
   is unused. */

#include <stddef.h>                       /* size_t */
#include "../utils/seq_realloc.h"          /* seq_realloc */

static seq_t *cache = NULL;       /* 1‑indexed cache of computed terms */
static size_t cache_cap = 0;       /* allocated capacity (≥ cached_size+1) */
static idx_t cached_size = 0;      /* largest n with valid cache[n] */

/* Grow the cache to contain indices up to n (inclusive). */
static void ensure_cached(idx_t n)
{
    if (n <= cached_size)
        return;                    /* already computed */

    /* Allocate or grow the underlying array. */
    size_t req_cap = (size_t)n + 1;    /* need indices 0..n */
    if (req_cap > cache_cap) {
        seq_t *tmp = seq_realloc(cache, cache_cap, req_cap);
        if (!tmp)                    /* allocation failed – silently abort */
            return;
        cache = tmp;
        cache_cap = req_cap;
    }

    /* Initialise the base values if they are missing. */
    if (cached_size < 1 && n >= 1) cache[1] = (seq_t)16;
    if (cached_size < 2 && n >= 2) cache[2] = (seq_t)139;
    if (cached_size < 3 && n >= 3) cache[3] = (seq_t)1202;
    if (cached_size < 4 && n >= 4) cache[4] = (seq_t)10409;

    /* Extend the sequence using the recurrence. */
    idx_t start = cached_size + 1;
    if (start < 5)                 /* skip the base indices already set */
        start = 5;

    for (idx_t i = start; i <= n; ++i) {
        cache[i] =
            (seq_t)10 * cache[i-1]
          - (seq_t)11 * cache[i-2]
          - (seq_t) 5 * cache[i-3]
          -          cache[i-4];
    }

    if (n > cached_size)
        cached_size = n;
}

/* Public function as required by the build. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n < 1, but safe */
    ensure_cached(n);
    return cache[n];
}
