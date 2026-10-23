/*
 * A186120  Number of (n+1) X 2 binary arrays with every 2 X 2 subblock
 *          diagonal sum less antidiagonal sum equal to some horizontal or
 *          vertical neighbor 2 X 2 subblock diagonal sum less antidiagonal
 *          sum.
 * https://oeis.org/A186120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-3) + 6*a(… -4) - 4*a(n-5) (valid for
 *            n>6) with initial values a(1)=0, a(2)=14, a(3)=18, a(4)=50,
 *            a(5)=74, a(6)=182
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */

static seq_t *cache = NULL;          /* 1‑based cache of computed terms */
static idx_t  cache_cap = 0;         /* allocated size minus 1   */
static idx_t  cache_max = 0;         /* highest index computed   */

/* Ensure the cache can hold indices up to 'need'.  Expands with realloc. */
static void ensure_capacity(idx_t need)
{
    if (need <= cache_cap)
        return;
    /* double size until large enough */
    idx_t new_cap = cache_cap >= 7 ? cache_cap * 2 : 7;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_cache)
        exit(1);   /* insufficient memory */
    cache = new_cache;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* initialize base table on first call */
    if (!cache) {
        ensure_capacity(6);               /* need space for indices 0..6   */
        /* index 0 unused */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)14;
        cache[3] = (seq_t)18;
        cache[4] = (seq_t)50;
        cache[5] = (seq_t)74;
        cache[6] = (seq_t)182;
        cache_max = 6;
    }

    /* If requested index already computed, return it */
    if (n <= cache_max)
        return cache[n];

    /* Ensure cache can hold up to n */
    ensure_capacity(n);

    /* Compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val = ((seq_t)3) * cache[i - 1];
        val += ((seq_t)6) * cache[i - 4];
        val -= ((seq_t)6) * cache[i - 3];
        val -= ((seq_t)4) * cache[i - 5];
        cache[i] = val;
    }
    cache_max = n;
    return cache[n];
}
