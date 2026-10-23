/*
 * A131871  a(n) = A131766(n) / 18.
 * https://oeis.org/A131871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 100*a(n-9) - 200*a(n-10) +
 *            100*a(n-11) initial values:
 *            a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=0,a(5)=1,a(6)=2,
 *            a(7)=3,a(8)=4,a(9)=10,a(10)=21,a(11)=32
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

static seq_t *cache = NULL;        /* memoised values */
static idx_t  cache_size = 0;      /* number of stored entries */

/* base values for n = 0 … 11 */
static const seq_t base_vals[12] = {
    0, 0, 0, 0, 0, 1, 2, 3, 4, 10, 21, 32
};

/* initialise cache with the base values */
static void init_cache(void)
{
    cache_size = 12;
    cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    assert(cache != NULL);
    for (idx_t i = 0; i < cache_size; ++i)
        cache[i] = base_vals[i];
}

/* extend the cache up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL)
        init_cache();

    if (n < cache_size)
        return;                     /* already stored */

    /* allocate larger array and recompute new entries */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    assert(new_cache != NULL);
    cache = new_cache;

    for (idx_t i = cache_size; i <= n; ++i)
    {
        /* recurrence is valid only for i >= 12, but cache_size >= 12 now */
        cache[i] =
            2 * cache[i-1]
          -      cache[i-2]
          + 100 * cache[i-9]
          - 200 * cache[i-10]
          + 100 * cache[i-11];
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
