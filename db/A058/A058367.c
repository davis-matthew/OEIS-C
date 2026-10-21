/*
 * A058367  Number of ways to cover (without overlapping) a ring lattice
 *          (necklace) of n sites with molecules that are 6 sites wide.
 * https://oeis.org/A058367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-6), a(1..5)=1, a(6)=7
 * Range:     u8 n <= 22, u16 n <= 44, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* array of computed terms, 1‑based indices */
static idx_t  cache_size = 0;      /* number of slots allocated          */
static idx_t  terms_computed = 0;  /* highest n with a(n) computed       */

/* Ensure cache has room for at least n entries */
static void ensure_cache(idx_t n)
{
    if (cache_size >= n)
        return;
    idx_t new_size = cache_size ? cache_size * 2 : 16;
    while (new_size < n)
        new_size <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache = tmp;
    cache_size = new_size;
}

/* Compute a(n) for the requested n, extending the cache if necessary. */
seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; n must be >=1; return 0 for safety */
        return (seq_t)0;

    if (terms_computed < n) {
        ensure_cache(n);
        for (idx_t i = terms_computed + 1; i <= n; ++i) {
            if (i <= 5)
                cache[i - 1] = (seq_t)1;
            else if (i == 6)
                cache[i - 1] = (seq_t)7;
            else
                cache[i - 1] = cache[i - 2] + cache[i - 7]; /* a(i-1) + a(i-6) */
        }
        terms_computed = n;
    }
    return cache[n - 1];
}
