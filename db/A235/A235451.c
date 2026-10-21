/*
 * A235451  Number of length n words on alphabet {0,1,2} of the form
 *          0^(i)1^(j)2^(k) such that i=j or j=k.
 * https://oeis.org/A235451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-3) - a(n-5) for n >= 5 with base values
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=3, a(4)=6
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoisation table */
static idx_t cache_len = 0;      /* number of entries currently defined */
static idx_t cache_cap = 0;      /* current capacity of cache */

/* initialise the cache with the first five terms */
static void init_cache(void)
{
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
    }
    if (cache_len < 5) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)6;
        cache_len = 5;
    }
}

/* enlarge the cache capacity if necessary */
static void ensure_capacity(idx_t n)
{
    idx_t required = n + 1;
    if (cache_cap >= required) return;
    idx_t new_cap = (cache_cap > 1) ? cache_cap : 16;
    while (new_cap < required) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (tmp == NULL) abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    init_cache();

    if (n < cache_len) return cache[n];

    ensure_capacity(n);

    for (idx_t i = cache_len; i <= n; ++i) {
        /* the recurrence holds for i >= 5; for i < 5 the base values are set in init_cache */
        cache[i] = cache[i - 2] + cache[i - 3] - cache[i - 5];
    }

    cache_len = n + 1;
    return cache[n];
}
