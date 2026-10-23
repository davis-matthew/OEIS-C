/*
 * A020945  a(2n+1) = |a(2n) - a(2n-1)|, a(2n) = a(n) + a(n-1).
 * https://oeis.org/A020945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n) = a(n)+a(n-1) * a(2n+1) = |a(2n)-a(2n-1)| * Initial
 *            values: a(0)=0, a(1)=1
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* table of computed terms, cache[0] = a(0) */
static size_t cache_cap = 0;         /* current capacity of cache   */
static size_t cache_size = 0;        /* highest index computed + 1   */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();                    /* out‑of‑memory */
        cache[0] = 0;                  /* a(0) */
        cache[1] = 1;                  /* a(1) */
        cache_size = 2;                /* indices 0 and 1 are ready */
    }

    /* expand cache if necessary */
    if ((size_t)n >= cache_cap)
    {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i)
    {
        if ((i & 1) == 0)            /* even: a(2k) = a(k)+a(k-1) */
        {
            size_t k = i >> 1;
            cache[i] = cache[k] + cache[k - 1];
        }
        else                         /* odd:  a(2k+1)=|a(2k)-a(2k-1)| */
        {
            size_t k    = (i - 1) >> 1;
            size_t idx1 = k << 1;    /* 2k   */
            size_t idx2 = idx1 - 1;  /* 2k-1 */
            seq_t v1 = cache[idx1];
            seq_t v2 = cache[idx2];
            cache[i] = (v1 > v2) ? (v1 - v2) : (v2 - v1);
        }
    }
    cache_size = (size_t)n + 1;       /* last computed index + 1 */

    return cache[(size_t)n];
}
