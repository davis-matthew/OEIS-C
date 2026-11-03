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
 * Validated: standard, pass, 38 terms through n = 996 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t get_cached(size_t idx)
{
    static size_t last_index = 0;
    static seq_t *cache = NULL;
    if (idx < last_index) return cache[idx];
    /* build up to idx */
    for (size_t i = last_index; i <= idx; ++i)
    {
        if (i == 0) cache[0] = (seq_t)5;
        else if (i == 1) cache[1] = (seq_t)30;
        else if (i == 2) cache[2] = (seq_t)200;
        else
        {
            seq_t v = (seq_t)8 * cache[(i-1)%3] - (seq_t)8 * cache[(i-2)%3] + cache[(i-3)%3];
            cache[i % 3] = v;
        }
    }
    last_index = idx + 1;
    return cache[idx];
}

static void ensure_capacity(size_t need)
{
    static size_t capacity = 0;
    static seq_t *cache = NULL;
    if (need <= capacity) return;
    size_t newcap = capacity ? capacity : 4;
    while (newcap <= need) newcap *= 2;
    cache = mem_resize(seq_t, cache, newcap);
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
        cache = mem_new(seq_t, 3);
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
        cache = mem_resize(seq_t, cache, newcap);
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
