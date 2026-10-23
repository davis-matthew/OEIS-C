/*
 * A190090  Diagonal sums of the triangular matrix A190088.
 * https://oeis.org/A190090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n‑1) + 2*a(n‑2) + 6*a(n‑3) – 3*a(n‑4) + a(n‑6), with
 *            a(0) = 1, a(1) = 1, a(2) = 4, a(3) = 16, a(4) = 42, a(5) = 137
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Static cache for memoisation */
static seq_t *cache   = NULL;  /* array of computed terms */
static size_t cap     = 0;     /* current allocated size */
static bool init_done = false;

/* Grow the cache to at least size 'need' (size_t) */
static void grow_cache(size_t need)
{
    if (need <= cap)
        return;
    size_t new_cap = cap ? cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failure – abort, as we cannot continue safely. */
        abort();
    }
    cache = tmp;
    cap   = new_cap;
}

/* Compute a(n) using the stated recurrence with memoisation. */
seq_t a(idx_t n)
{
    /* Ensure cache can hold the term up to n */
    grow_cache((size_t)n + 1);

    /* Initialise first six values if not already done. */
    if (!init_done) {
        if (cap < 6)
            grow_cache(6);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)16;
        cache[4] = (seq_t)42;
        cache[5] = (seq_t)137;
        init_done = true;
    }

    /* Compute missing terms up to n */
    for (size_t i = 6; i <= (size_t)n; ++i) {
        seq_t t = cache[i - 1] * 2 + cache[i - 2] * 2;
        t = t + cache[i - 3] * 6;
        t = t - cache[i - 4] * 3;
        t = t + cache[i - 6];
        cache[i] = t;
    }

    return cache[(size_t)n];
}
