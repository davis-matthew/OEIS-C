/*
 * A081015  a(n) = Lucas(4n+3) + 1, or 5*Fibonacci(2n+1)*Fibonacci(2n+2).
 * https://oeis.org/A081015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(4n+3)+1 = 5*Fib(2n+1)*Fib(2n+2) Recurrence: a(0)=5,
 *            a(1)=30, a(2)=200, a(n)=8*a(n-1)-8*a(n-2)+a(n-3) (n≥3)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   get_cached
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_cached.h"

static void ensure_capacity(size_t need)
{
    static size_t capacity = 0;
    static seq_t *cache = NULL;
    if (need <= capacity) return;
    size_t newcap = capacity ? capacity : 4;
    while (newcap <= need) newcap *= 2;
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    capacity = newcap;
}

seq_t a(idx_t n)
{
    /* n is ≥0; cast to size_t for indexing */
    size_t idx = (size_t)n;
    static size_t cache_capacity = 0;
    static seq_t *cache = NULL;
    static size_t cache_last = 0; /* number of values stored (up to last index +1) */

    if (cache == NULL) {
        /* allocate initial cache of 3 entries */
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)30;
        cache[2] = (seq_t)200;
        cache_capacity = 3;
        cache_last = 3; /* indices 0..2 are valid */
    }

    if (idx < 3) return cache[idx];

    /* ensure cache up to idx */
    if (idx >= cache_capacity) {
        size_t newcap = cache_capacity ? cache_capacity : 4;
        while (newcap <= idx) newcap *= 2;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache_capacity = newcap;
    }

    /* compute missing values */
    for (size_t i = cache_last; i <= idx; ++i)
    {
        seq_t v = (seq_t)8 * cache[i-1] - (seq_t)8 * cache[i-2] + cache[i-3];
        cache[i] = v;
    }
    cache_last = idx + 1;
    return cache[idx];
}
