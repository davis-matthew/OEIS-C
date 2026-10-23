/*
 * A122972  a(1) = 1, a(2) = 2; for n>2, a(n+1) = a(n)*(n-1) + a(n-1)*n.
 * https://oeis.org/A122972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2; for n>2, a(n) = a(n-1)*(n-2) + a(n-2)*(n-1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;           /* array of computed terms */
static idx_t  cache_len = 0;          /* number of terms currently stored */
static idx_t  cache_cap = 0;          /* capacity of cache */

/* grow the cache to at least new_cap terms */
static void ensure_cache_cap(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    idx_t cap = cache_cap > 0 ? cache_cap : 1;
    while (cap < new_cap)
        cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!new_ptr)
        abort();               /* cannot proceed without storage */
    cache = new_ptr;
    cache_cap = cap;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined before offset, but guard */
        return 0;

    /* special cases */
    if (n == 1)
        return 1;
    if (n == 2)
        return 2;

    if (n > cache_len)
    {
        ensure_cache_cap(n);          /* make space */

        /* initialise first two terms if necessary */
        if (cache_len == 0)
        {
            cache[0] = (seq_t)1;       /* a(1) */
            cache_len = 1;
        }
        if (cache_len == 1)
        {
            cache[1] = (seq_t)2;       /* a(2) */
            cache_len = 2;
        }

        /* compute missing terms up to n */
        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            idx_t idx = i - 1;          /* 0‑based index for a(i) */
            seq_t a_prev1 = cache[idx - 1]; /* a(i-1)   */
            seq_t a_prev2 = cache[idx - 2]; /* a(i-2)   */
            seq_t mul1 = a_prev1 * (seq_t)(i - 2);
            seq_t mul2 = a_prev2 * (seq_t)(i - 1);
            cache[idx] = mul1 + mul2;
        }
        cache_len = n;
    }
    return cache[n - 1];
}
