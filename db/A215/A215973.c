/*
 * A215973  a(0) = 1, for n > 0: a(n) = Sum_{k=0..n-1} a(k) * (1 + a(n-1-k)).
 * https://oeis.org/A215973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=0..n-1} a(k) * (1 + a(n-1-k)) for n > 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine keeps a static cache so that calls with arbitrary indices
   (and out of order) are fast. The cache is grown in chunks. */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdint.h>   /* for testing with uint64_t */

static seq_t *cache   = NULL;   /* dynamic array holding computed terms   */
static idx_t  cap     = 0;      /* current capacity of the cache         */
static idx_t  ncalc   = 0;      /* number of terms stored (cache[0..ncalc-1]) */

/* Ensure the cache can store at least (needed) entries */
static void grow_cache(idx_t needed)
{
    if (cap < needed)
    {
        idx_t new_cap = (cap == 0 ? 4 : cap * 2);
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                    /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* We need to be able to access indices up to n inclusive */
    grow_cache(n + 1);

    /* First term, if the cache is empty */
    if (ncalc == 0)
    {
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        ncalc    = 1;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = ncalc; i <= n; ++i)
    {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k)
        {
            seq_t term = cache[k] * (cache[i - 1 - k] + (seq_t)1);
            sum += term;
        }
        cache[i] = sum;
    }

    ncalc = n + 1;   /* we have computed up to index n */
    return cache[n];
}
