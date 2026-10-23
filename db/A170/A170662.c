/*
 * A170662  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>0,
   a(n) = 27*∑_{i=1}^{48} a(n−i) + 378*a(n−49) + c(n),
   where
       c(0)=1,
       c(1)…c(48)=2,
       c(49)=1,
       c(n)=0 for n>49. */
#include <stdlib.h>

static seq_t *cache = NULL;   /* memoised values */
static size_t cache_len = 0;  /* number of values currently stored */
static size_t cache_cap = 0;  /* allocated size */

/* Ensure cache holds terms up to at least n.  */
static void ensure_cache(idx_t n)
{
    if (cache_len > 0 && (size_t)n < cache_len)
        return;

    /* enlarge storage if necessary */
    if ((size_t)n + 1 > cache_cap)
    {
        seq_t *new_cache =
            (seq_t*)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        if (!new_cache) abort();          /* allocation failed */
        cache      = new_cache;
        cache_cap  = (size_t)n + 1;
    }

    /* fill missing terms */
    for (size_t k = cache_len; k <= (size_t)n; ++k)
    {
        if (k == 0)               /* a(0) = 1 */
        {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = 0;

        /* sum 27 * a(k-i) for i = 1 … 48 (only up to k) */
        size_t maxi = (k < 48 ? k : 48);
        for (size_t i = 1; i <= maxi; ++i)
            sum += (seq_t)27 * cache[k - i];

        /* add 378 * a(k-49) when k ≥ 49 */
        if (k >= 49)
            sum += (seq_t)378 * cache[k - 49];

        /* add the constant term c(k) */
        if (k <= 48)
            sum += (seq_t)2;
        else if (k == 49)
            sum += (seq_t)1;
        /* else k > 49 → no constant term */

        cache[k] = sum;
    }

    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
