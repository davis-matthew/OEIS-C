/*
 * A229464  Binomial transform of (2*n + 1)!.
 * https://oeis.org/A229464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..n} binomial(n,k)*(2*k+1)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=7,
   a(n) = 1 + 2*n*(2*n + 1)*a(n-1) - 2*n*(2*n - 2)*a(n-2)   (n≥2) */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* array of computed values        */
static size_t cache_size = 0;    /* number of values currently stored */
static size_t cache_cap  = 0;    /* allocated capacity                */

/* Ensure the cache can access indices up to 'needed-1'. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In a real build an error handler could be used. */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Make sure the cache can hold index n. */
    grow_cache((size_t)n + 1);

    /* initialise base cases if they are not set yet */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }
    if (n == 0)
        return cache[0];
    if (cache_size == 1) {
        cache[1] = (seq_t)7;
        cache_size = 2;
    }
    if (n == 1)
        return cache[1];
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* compute missing values up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t two_i       = (seq_t)i * 2;                /* 2*i          */
        seq_t two_i_plus  = two_i + 1;                   /* 2*i + 1      */
        seq_t two_i_minus = two_i - 2;                   /* 2*i - 2      */

        seq_t term1 = two_i * two_i_plus * cache[i-1];   /* 2*i*(2*i+1)*a(i-1) */
        seq_t term2 = two_i * two_i_minus * cache[i-2];  /* 2*i*(2*i-2)*a(i-2) */

        seq_t val = term1 + 1;
        val -= term2;                                    /* a(i) = 1 + term1 - term2 */

        cache[i] = val;
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
