/*
 * A209377  1/4 the number of (n+1) X 4 0..2 arrays with every 2 X 2 subblock
 *          having distinct edge sums.
 * https://oeis.org/A209377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-2) - 12*a(n-4) (n ≥ 5) a(1)=16, a(2)=34, a(3)=64,
 *            a(4)=156
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 23, u64 n <= 48, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* 1‑based array of sequence values */
static size_t cache_size = 0;        /* current allocated length */
static idx_t cache_nmax = 0;         /* highest n for which cache[n] is valid */

/* ------------------------------------------------------------------ */
/*  Ensure cache can hold at least up to idx_t n (1‑based).            */
static void expand_cache(idx_t n)
{
    if (cache_nmax >= n) return;
    size_t new_size = (n + 1) * 2;        /* allocate a bit extra for future growth */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – terminate – sequence undefined beyond this point */
        abort();
    }
    cache = new_cache;
    /* initialize new slots (except cache[0], which is unused) */
    for (size_t i = cache_nmax + 1; i < new_size; ++i)
        cache[i] = 0;
    cache_size = new_size;

    /* set the base values if they have not yet been set */
    if (cache_size > 0) {
        if (cache_size > 1 && cache[1] == 0) cache[1] = (seq_t)16;
        if (cache_size > 2 && cache[2] == 0) cache[2] = (seq_t)34;
        if (cache_size > 3 && cache[3] == 0) cache[3] = (seq_t)64;
        if (cache_size > 4 && cache[4] == 0) cache[4] = (seq_t)156;
    }
    /* compute missing values up to n */
    for (idx_t k = cache_nmax + 1; k <= n; ++k) {
        if (k < 5) {
            /* already set above */
            continue;
        }
        /* recurrence: 8*a(k-2) - 12*a(k-4) */
        seq_t term = (seq_t)8 * cache[k - 2] - (seq_t)12 * cache[k - 4];
        cache[k] = term;
    }
    cache_nmax = n;
}
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */
    if (!cache) {
        /* initial allocation – enough for base terms */
        expand_cache(4);
    }
    if (cache_nmax < n) {
        expand_cache(n);
    }
    return cache[n];
}
