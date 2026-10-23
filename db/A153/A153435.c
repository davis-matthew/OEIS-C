/*
 * A153435  Numbers with 2n binary digits where every run length is 2, written
 *          in binary.
 * https://oeis.org/A153435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 100*a(n-1) + a(n-2) - 100*a(n-3) a(1)=11, a(2)=1100,
 *            a(3)=110011 Offset: 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t cache_len = 0;
static seq_t *cache = NULL;          /* cache[0] unused */

/* Initialise the cache with the first three terms. */
static void init_cache(void)
{
    cache_len = 3;
    cache = (seq_t *)malloc((cache_len + 1) * sizeof(seq_t));
    if (!cache)
        return;                       /* allocation failed – caller will fail gracefully */
    cache[0] = (seq_t)0;                 /* unused */
    cache[1] = (seq_t)11;
    cache[2] = (seq_t)1100;
    cache[3] = (seq_t)110011;
}

/* Sequence generator. */
seq_t a(idx_t n)
{
    if (n < 1)                         /* below the offset */
        return (seq_t)0;

    if (!cache)                        /* first call */
        init_cache();

    if (!cache)                        /* allocation failed */
        return (seq_t)0;

    if (n <= cache_len)                /* already cached */
        return cache[n];

    /* Grow the cache to accommodate index n. */
    size_t new_size = (size_t)(n + 1);  /* indices 0 .. n */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)                          /* realloc failed – allocate fresh buffer */
    {
        tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!tmp)                      /* allocation failed again */
            return (seq_t)0;
        /* copy existing data */
        for (idx_t i = 1; i <= cache_len; ++i)
            tmp[i] = cache[i];
    }
    cache = tmp;

    /* Compute any missing terms using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i)
        cache[i] = (seq_t)100 * cache[i - 1] + cache[i - 2] - (seq_t)100 * cache[i - 3];

    cache_len = n;
    return cache[n];
}
