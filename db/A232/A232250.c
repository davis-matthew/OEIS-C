/*
 * A232250  Number of (n+1) X (1+1) 0..2 arrays with every element both >= and
 *          <= some horizontal or antidiagonal neighbor.
 * https://oeis.org/A232250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A232250
 *  a(1) = 9, a(2) = 37, a(3) = 163, a(4) = 723
 *  a(n) = 4*a(n-1) + 2*a(n-2) - a(n-4)   for n ≥ 5
 *  Padding: index 0 is unused.
 */
#include <stdlib.h>

/* static cache (index 0 unused) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Grow the cache so that it can hold indices up to idx (inclusive) */
static void ensure_cache(idx_t idx)
{
    if (idx == 0) return;                 /* nothing to do for the offset */
    size_t needed = (size_t)idx + 1;      /* because we keep entry 0 unused */

    /* Allocate / reallocate memory if necessary */
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap : needed;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;                /* allocation failed – caller will fall back to 0 */
        /* Initialize new slots to 0 */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Base values – set once if they are still 0 */
    if (idx >= 1 && cache[1] == (seq_t)0) cache[1] = (seq_t)9;
    if (idx >= 2 && cache[2] == (seq_t)0) cache[2] = (seq_t)37;
    if (idx >= 3 && cache[3] == (seq_t)0) cache[3] = (seq_t)163;
    if (idx >= 4 && cache[4] == (seq_t)0) cache[4] = (seq_t)723;

    /* Fill in values using the recurrence */
    for (size_t i = 5; i <= (size_t)idx; ++i) {
        if (cache[i] == (seq_t)0) {
            seq_t v = cache[i - 1] * (seq_t)4
                    + cache[i - 2] * (seq_t)2
                    - cache[i - 4];
            cache[i] = v;
        }
    }
}

/* External‑linkage function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 0 */
    ensure_cache(n);
    if (cache && (size_t)n < cache_cap) return cache[(size_t)n];
    return (seq_t)0;               /* safety – should never happen if allocation succeeded */
}
