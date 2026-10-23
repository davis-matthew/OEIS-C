/*
 * A180763  Half the number of n X 3 binary arrays with each element equal to
 *          at least one neighbor.
 * https://oeis.org/A180763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            5*a(n-1)+8*a(n-2)-5*a(n-3)-10*a(n-4)-a(n-5)+3*a(n-6)+4*a(n-7)-2*a(n-8)
 *            Initial values:
 *            a(1)=1,a(2)=11,a(3)=63,a(4)=390,a(5)=2385,a(6)=14621,a(7)=89602,a(8)=549125
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* Ensure the cache holds the first eight terms. */
static void init_cache(void)
{
    if (cache == NULL)
    {
        cache_cap = 8;
        cache = alloc_seq_array(cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)63;
        cache[3] = (seq_t)390;
        cache[4] = (seq_t)2385;
        cache[5] = (seq_t)14621;
        cache[6] = (seq_t)89602;
        cache[7] = (seq_t)549125;
        cache_len = 8;
    }
}

/* Grow the cache to at least 'need' elements. */
static void grow_cache(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : need;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_cache = alloc_seq_array(new_cap);
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    cache = new_cache;
    cache_cap = new_cap;
}

/* OEIS sequence A180763 */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1; return 0 */

    init_cache();

    if (n <= cache_len)
        return cache[n - 1];

    /* Prepare cache large enough. */
    grow_cache(n);

    /* Compute terms from cache_len+1 up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        seq_t term = 5 * cache[i - 2]          /* 5*a(i-1) */
                  + 8 * cache[i - 3]          /* 8*a(i-2) */
                  - 5 * cache[i - 4]          /* -5*a(i-3) */
                  -10 * cache[i - 5]          /* -10*a(i-4) */
                  -  cache[i - 6]             /* -a(i-5) */
                  + 3 * cache[i - 7]          /* +3*a(i-6) */
                  + 4 * cache[i - 8]          /* +4*a(i-7) */
                  - 2 * cache[i - 9];         /* -2*a(i-8) */
        cache[i - 1] = term;
    }

    cache_len = n;
    return cache[n - 1];
}
