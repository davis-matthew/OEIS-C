/*
 * A164760  Number of n X 3 1..4 arrays with all 1s connected, all 2s
 *          connected, all 3s connected, all 4s connected, 1 in the upper left
 *          corner, 2 in the upper right corner, 3 in the lower left corner, 4
 *          in the lower right corner, and with no element having more than 3
 *          neighbors with the same value.
 * https://oeis.org/A164760
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 42*a(n-2) + 96*a(n-3) - 129*a(n-4) +
 *            102*a(n-5) - 44*a(n-6) + 8*a(n-7) a(2)=8, a(3)=104, a(4)=670,
 *            a(5)=3096, a(6)=11786, a(7)=39564, a(8)=121678
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 19, u64 n <= 48, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* cached values of a(n) */
static idx_t  cached_max = 0;        /* largest n computed so far */
static size_t cache_size = 0;        /* allocated size of cache array */

/* initialise the cache with the known starting values */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    /* Ensure space at least up to index 8 (a(8)). */
    size_t init_size = 9;                         /* indices 0..8 */
    cache = (seq_t *)calloc(init_size, sizeof(seq_t));
    if (!cache)
        return;                                  /* allocation failure – caller will fail */

    cache[2] = (seq_t)8;
    cache[3] = (seq_t)104;
    cache[4] = (seq_t)670;
    cache[5] = (seq_t)3096;
    cache[6] = (seq_t)11786;
    cache[7] = (seq_t)39564;
    cache[8] = (seq_t)121678;
    cached_max = 8;
    cache_size = init_size;
}

/* grow the cache array so that it can hold indices up to n */
static void ensure_cache_size(idx_t n)
{
    if ((idx_t)cache_size > n)
        return;

    size_t new_size = (size_t)(n + 1);
    if (new_size < 9)
        new_size = 9;

    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache)
        return;           /* allocation failure – caller will fail */

    memcpy(new_cache, cache, cache_size * sizeof(seq_t));
    /* initialise added slots to zero */
    memset(new_cache + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
    free(cache);
    cache     = new_cache;
    cache_size = new_size;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    init_cache();
    if ((idx_t)cache_size <= n)
        ensure_cache_size(n);

    if (n <= cached_max)
        return cache[n];

    /* compute missing values up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i)
    {
        /* for i >= 9 the recurrence applies */
        if (i >= 9)
        {
            seq_t sum1 = (seq_t)10 * cache[i-1];
            sum1 += (seq_t)96 * cache[i-3];
            sum1 += (seq_t)102 * cache[i-5];
            sum1 += (seq_t)8  * cache[i-7];

            seq_t sum2 = (seq_t)42 * cache[i-2];
            sum2 += (seq_t)129 * cache[i-4];
            sum2 += (seq_t)44  * cache[i-6];

            cache[i] = sum1 - sum2;     /* sum1 > sum2 always */
        }
    }

    cached_max = n;
    return cache[n];
}
