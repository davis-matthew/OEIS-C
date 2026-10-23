/*
 * A181443  Solutions a(n) to (r(n)-5)*(r(n)-6) = 21 *a(n)*(a(n)-1).
 * https://oeis.org/A181443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(n) = 111*a(n-4) -
 *            111*a(n-8) + a(n-12) with initial values a(0)=1, a(1)=2, a(2)=5,
 *            a(3)=22, a(4)=34, a(5)=161, a(6)=494, a(7)=2365, a(8)=3685,
 *            a(9)=17654, a(10)=54281, a(11)=260074. This implementation
 *            memoizes computed values so that any order of calls is s...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* stored sequence values */
static size_t cache_len = 0;       /* number of values currently stored */

static seq_t init[12] = {
    (seq_t)1,   (seq_t)2,   (seq_t)5,   (seq_t)22,
    (seq_t)34,  (seq_t)161, (seq_t)494, (seq_t)2365,
    (seq_t)3685,(seq_t)17654,(seq_t)54281,(seq_t)260074
};

/* ensure the cache holds at least n+1 entries and
   fill entries that are not yet computed */
static void grow_cache_until(idx_t n)
{
    if (n < (idx_t)cache_len)
        return;                     /* already present */

    size_t new_len = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!cache) abort();            /* allocation failed */

    /* initialise new entries */
    size_t i;
    for (i = cache_len; i < new_len; ++i)
    {
        if (i < 12)
        {
            cache[i] = init[i];
        }
        else
        {
            /* recurrence: a(i) = 111*a(i-4) - 111*a(i-8) + a(i-12) */
            seq_t term1 = (seq_t)111 * cache[i - 4];
            seq_t term2 = (seq_t)111 * cache[i - 8];
            cache[i] = term1 - term2 + cache[i - 12];
        }
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!cache)
    {
        cache = (seq_t *)malloc(12 * sizeof(seq_t));
        if (!cache) abort();        /* allocation failed */
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init[i];
        cache_len = 12;
    }

    /* extend cache if necessary */
    if (n >= (idx_t)cache_len)
        grow_cache_until(n);

    return cache[(size_t)n];
}
