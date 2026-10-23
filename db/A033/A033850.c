/*
 * A033850  Numbers whose prime factors are 3 and 7.
 * https://oeis.org/A033850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 100, u64 n <= 429,
 *            u128 n <= 1778, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A033850: a(n) = minimal integer >0 with only prime factors 3 and 7, sorted ascending (a(1)=21). */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of generated terms */
static size_t cache_cap = 0;  /* allocated capacity of cache */
static idx_t cache_len = 0;   /* number of terms currently stored in cache */
static idx_t ptr3 = 0;        /* index for next multiple by 3   */
static idx_t ptr7 = 0;        /* index for next multiple by 7   */

/* Ensure that the cache can hold at least 'need' elements. */
static void ensure_capacity(idx_t need)
{
    if ((idx_t)cache_cap < need)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)need)
            new_cap = (size_t)need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* allocation failed – terminate program */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the n-th term (n >= 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined for n < 1, but return 0 */

    /* Initialize the cache on first call. */
    if (cache_len == 0)
    {
        ensure_capacity(1);
        cache[0] = (seq_t)21;           /* 3^1 * 7^1 */
        cache_len = 1;
        ptr3 = 0;
        ptr7 = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        seq_t next3 = cache[ptr3] * (seq_t)3;
        seq_t next7 = cache[ptr7] * (seq_t)7;
        seq_t next  = (next3 < next7) ? next3 : next7;

        if (next3 == next)
            ++ptr3;
        if (next7 == next)
            ++ptr7;

        ensure_capacity(cache_len + 1);
        cache[cache_len++] = next;
    }

    return cache[n - 1];
}
