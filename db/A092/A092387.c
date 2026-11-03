/*
 * A092387  a(n) = Fibonacci(2*n+1) + Fibonacci(2*n-1) + 2.
 * https://oeis.org/A092387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n+1) + Fibonacci(2*n-1) + 2 (equivalent
 *            recurrence: a(0)=4, a(1)=5, a(2)=9,
 *            a(n)=4*a(n-1)-4*a(n-2)+a(n-3) for n≥3)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 729 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* calloc, realloc, abort */
#include "../utils/mem.h"

static seq_t *cache   = NULL;   /* cached terms a(0)…a(k) */
static idx_t  cache_len = 0;    /* size of allocated cache */

/* Ensure that cache[n] is computed (call with desired n). */
static void ensure_cache(idx_t n)
{
    if (cache_len == 0)          /* initialise base terms 0…2 */
    {
        cache_len = 3;
        cache = mem_new(seq_t, cache_len);
        if (!cache) abort();
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)9;
    }

    if (n >= cache_len)          /* extend cache up to n */
    {
        size_t new_len = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_len);
        if (!tmp) abort();

        /* Compute new terms using the recurrence. */
        for (idx_t i = cache_len; i <= n; ++i)
            tmp[i] = 4 * tmp[i-1] - 4 * tmp[i-2] + tmp[i-3];

        cache = tmp;
        cache_len = new_len;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
