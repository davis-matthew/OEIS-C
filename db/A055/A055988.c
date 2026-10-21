/*
 * A055988  Sequence is its own 4th difference.
 * https://oeis.org/A055988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) Initial values:
 *            a(1)=1, a(2)=2, a(3)=7, a(4)=26
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* malloc, free */

/* Static cache for already-computed terms */
static seq_t *cache = NULL;    /* 1‑indexed array: cache[1] contains a(1) */
static idx_t  cache_len = 0;   /* highest index currently stored          */
static idx_t  cache_cap = 0;   /* allocated capacity of the array         */

/* grow the cache array to accommodate at least cap elements (1‑indexed) */
static void ensure_capacity(idx_t cap)
{
    if (cap <= cache_cap) return;

    /* choose a new capacity: at least cap, and a power of two */
    idx_t new_cap = cache_cap > 0 ? cache_cap * 2 : 8;
    while (new_cap < cap) new_cap *= 2;

    seq_t *new_arr = (seq_t *)malloc((size_t)(new_cap + 1) * sizeof(seq_t));
    if (!new_arr) return;             /* out of memory; caller will see error  */

    /* copy existing data */
    for (idx_t i = 1; i <= cache_len; ++i) new_arr[i] = cache[i];

    /* free old buffer */
    free(cache);

    cache     = new_arr;
    cache_cap = new_cap;
}

/* OEIS A055988 – Sequence is its own 4th difference */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined, safe fallback          */

    /* initialise first four terms on first call */
    if (cache_len == 0) {
        ensure_capacity(4);
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)26;
        cache_len = 4;
    }

    /* grow cache if needed */
    if (n > cache_cap) ensure_capacity(n);

    /* compute missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* recurrence: a[i] = 5*a[i-1] - 6*a[i-2] + 4*a[i-3] - a[i-4] */
        seq_t term = (seq_t)5 * cache[i - 1]
                    - (seq_t)6 * cache[i - 2]
                    + (seq_t)4 * cache[i - 3]
                    - cache[i - 4];
        cache[i] = term;
    }

    if (cache_len < n) cache_len = n;
    return cache[n];
}
