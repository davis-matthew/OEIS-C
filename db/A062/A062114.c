/*
 * A062114  a(n) = 2*Fibonacci(n) - (1 - (-1)^n)/2.
 * https://oeis.org/A062114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*Fibonacci(n) - (1-(-1)^n)/2. Recurrence used: a(0)=0,
 *            a(1)=1, a(n) = a(n-1) + a(n-2) + ((1+(-1)^n)/2) => add 1 when n
 *            is even, add 0 when n is odd
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_size = 0;    /* number of terms stored in cache */
static size_t cache_cap  = 0;    /* current capacity of cache */

static void ensure_cache(size_t n)
{
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        cache_size = 2;
    }

    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= n + 1)
            new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    for (size_t i = cache_size; i <= n; ++i) {
        seq_t val = cache[i - 1] + cache[i - 2];
        /* add (1+(-1)^i)/2 : 1 if i even, 0 if i odd */
        if ((i & 1) == 0)
            ++val;
        cache[i] = val;
    }
    if (n + 1 > cache_size)
        cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache((size_t)n);
    return cache[(size_t)n];
}
