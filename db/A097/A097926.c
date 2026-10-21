/*
 * A097926  Number of (n,4) Freiman-Wyner sequences.
 * https://oeis.org/A097926
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-5) for n >= 10 Initial values (offset 5):
 *            a(5)=18, a(6)=36, a(7)=70, a(8)=134, a(9)=258 The function
 *            caches computed values to allow arbitrary non‑monotone calls
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[5] = {18, 36, 70, 134, 258};

static seq_t *cache = NULL;       /* cache[n-5] = a(n) for n >= 5          */
static size_t cache_len = 0;      /* number of entries currently cached   */
static idx_t   max_n_cached = 4;  /* smallest n with a(n) unmapped yet    */

/* Compute a(n) for n >= 5, returning the value or 0 for n < 5 */
seq_t a(idx_t n)
{
    if (n < 5)        /* behaviour below offset is irrelevant */
        return 0;

    /* If already computed, just return it. */
    if (n <= max_n_cached)
        return cache[n - 5];

    /* Need to extend the cache to cover n. */
    size_t needed_len = (size_t)(n - 5 + 1);          /* number of terms needed */
    seq_t *tmp = (seq_t *)realloc(cache, needed_len * sizeof(seq_t));
    cache = tmp;                                      /* may move memory */

    /* Fill any new terms from the current end up to n. */
    for (size_t i = cache_len; i < needed_len; ++i)
    {
        if (i < 5)                            /* initial values */
            cache[i] = init_vals[i];
        else                                  /* recurrence a(n) = 2*a(n-1) - a(n-5) */
            cache[i] = cache[i - 1] + cache[i - 1] - cache[i - 5];
    }

    /* Update bookkeeping. */
    cache_len   = needed_len;
    max_n_cached = n;

    return cache[n - 5];
}
