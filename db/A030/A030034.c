/*
 * A030034  a(n+1) = Sum_{k=0..floor(n/4)} a(k) * a(n-k).
 * https://oeis.org/A030034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n+1) = Sum_{k=0..⌊n/4⌋} a(k) * a(n-k)
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 37, u64 n <= 67, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array holding computed a(n) values          */
static size_t cache_len = 0;     /* capacity of the cache (number of slots)    */
static idx_t  computed = 0;      /* largest index already computed (0 if none) */

/* ensure that the cache can store indices 0..n */
static void
ensure_cache(idx_t n)
{
    if (cache_len <= (size_t)n)
    {
        size_t new_len = cache_len ? cache_len * 2 : 32;
        while (new_len <= (size_t)n)
            new_len *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();         /* out of memory – terminate */
        /* zero new entries */
        for (size_t i = cache_len; i < new_len; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_len = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    ensure_cache(n);

    /* initialise a(0) if it hasn't been set yet */
    if (computed == 0 && cache[0] == (seq_t)0)
    {
        cache[0] = (seq_t)1;
        computed = 0;
    }

    /* compute any missing values up to n */
    if (computed < n)
    {
        for (idx_t cur = computed + 1; cur <= n; ++cur)
        {
            idx_t up = (cur - 1) / 4;   /* floor((cur-1)/4) */
            seq_t total = (seq_t)0;
            for (idx_t k = 0; k <= up; ++k)
                total += cache[k] * cache[cur - 1 - k];
            cache[cur] = total;
        }
        computed = n;
    }

    return cache[n];
}
