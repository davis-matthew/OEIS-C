/*
 * A181137  The number of ways to color n balls in a row with 3 colors with no
 *          color runs having lengths greater than 3.
 * https://oeis.org/A181137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 2*(a(n)+a(n+1)+a(n+2)), a(0)=1, a(1)=3, a(2)=9, a(3)=27
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic cache of computed values */
static size_t cache_len = 0;  /* current number of stored values */
static size_t cache_cap = 0;  /* allocated capacity of cache */

/* Ensure the cache holds at least n+1 entries, initialize base values if empty. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 4;                      /* at least for indices 0..3 */
        cache_len  = 4;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 3;
        cache[2] = 9;
        cache[3] = 27;
    }

    if (cache_len <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1); /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* recurrence: a(i) = 2*(a(i-3)+a(i-2)+a(i-1)) */
            seq_t sum = cache[i - 3] + cache[i - 2] + cache[i - 1];
            cache[i] = sum * 2;
        }
        cache_len = (size_t)n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
