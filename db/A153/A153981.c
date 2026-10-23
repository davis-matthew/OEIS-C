/*
 * A153981  a(n) = 36*Fibonacci(2*n+1) - 4.
 * https://oeis.org/A153981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*Fibonacci(2*n+1) - 4 Recurrence: a(0) = 32, a(1) = 68,
 *            a(n) = 3*a(n-1) - a(n-2) + 4 for n >= 2
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_last = (idx_t)-1;

static void ensure_cache_size(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_cache_size(n);

    if (cache_last == (idx_t)-1) {
        cache[0] = (seq_t)32;
        cache[1] = (seq_t)68;
        cache_last = 1;
    }

    if (n <= cache_last)
        return cache[n];

    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        cache[i] = 3 * t1 - t2 + (seq_t)4;
    }
    cache_last = n;
    return cache[n];
}
