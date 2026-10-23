/*
 * A129857  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+857)^2 = y^2.
 * https://oeis.org/A129857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 28, u64 n <= 65, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-3) - a(n-6) + 1714  (for n > 6)
   Initial values: a(1)=0, a(2)=235, a(3)=1696, a(4)=2571, a(5)=3796, a(6)=12075
   (1-based indexing) */

#include <stdlib.h>

static seq_t *cache = NULL;       /* 1‑based cache array, cache[0] unused */
static size_t cap = 0;            /* allocated size of cache (indices 0..cap-1) */
static idx_t memoized = 0;        /* largest index that has been computed */

/* Ensure cache can hold indices 0..n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;                /* allocation failed – leave cache unchanged */
        cache = tmp;
        cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n <= 0 */

    /* Initialize the first six terms if not yet done */
    if (memoized < 6) {
        ensure_cache(6);
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)235;
        cache[3] = (seq_t)1696;
        cache[4] = (seq_t)2571;
        cache[5] = (seq_t)3796;
        cache[6] = (seq_t)12075;
        memoized = 6;
    }

    if (n <= memoized) return cache[(size_t)n];

    /* Grow cache to size n and compute missing terms */
    ensure_cache(n);
    for (idx_t i = memoized + 1; i <= n; ++i) {
        cache[(size_t)i] = (seq_t)6 * cache[(size_t)(i - 3)]
                          - cache[(size_t)(i - 6)]
                          + (seq_t)1714;
    }
    memoized = n;
    return cache[(size_t)n];
}
