/*
 * A189197  Number of 4 X n binary arrays without the pattern 0 0 1 vertically
 *          or antidiagonally.
 * https://oeis.org/A189197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 14*a(n-2) - 52*a(n-3) + 82*a(n-4) + 16*a(n-5)
 *            - 56*a(n-6) + 16*a(n-7), n >= 8 Initial values (a(1) .. a(7)):
 *            12, 144, 1494, 15326, 156564, 1598444, 16316636 Offset: 1 (a(1)
 *            is the first term)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                     /* offset 1 – undefined */

    /* static cache for memoization */
    static seq_t *cache   = NULL;   /* array of terms, 0‑based: cache[0]=a(1) */
    static idx_t  cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* capacity of the array           */
    const idx_t init_terms = 7;      /* a(1) .. a(7) */

    /* Lazy initialization of the cache */
    if (cache == NULL)
    {
        cache_cap = init_terms;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failed */
        cache[0] = (seq_t)12;
        cache[1] = (seq_t)144;
        cache[2] = (seq_t)1494;
        cache[3] = (seq_t)15326;
        cache[4] = (seq_t)156564;
        cache[5] = (seq_t)1598444;
        cache[6] = (seq_t)16316636;
        cache_len = init_terms;
    }

    /* Grow the cache if requested size exceeds current capacity */
    if (n > (idx_t)cache_cap)
    {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 8;
        while ((idx_t)new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms only if n is larger than what we have stored */
    if (n > cache_len)
    {
        for (idx_t pos = cache_len; pos < n; ++pos)   /* pos = 0‑based index of a(pos+1) */
        {
            /* pos >= 7 (i.e., n >= 8) here because the first 7 terms are known */
            seq_t val = 12 * cache[pos-1];
            val -= 14 * cache[pos-2];
            val -= 52 * cache[pos-3];
            val += 82 * cache[pos-4];
            val += 16 * cache[pos-5];
            val -= 56 * cache[pos-6];
            val += 16 * cache[pos-7];
            cache[pos] = val;
        }
        cache_len = n;
    }

    return cache[n-1];   /* a(n) corresponds to cache[n-1] */
}
