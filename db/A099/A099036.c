/*
 * A099036  a(n) = 2^n - Fibonacci(n).
 * https://oeis.org/A099036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A099036
 *  a(n) = 2^n - Fibonacci(n)  (F(0)=0, F(1)=1)
 *  Recurrence: a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3)
 *    initial values: a(0)=1, a(1)=1, a(2)=3
 *
 * Uses a simple memoised recurrence; works for any order of calls.
 */

#include <stdlib.h>   /* malloc, realloc, exit, size_t */

static seq_t *cache = NULL;    /* memoisation array */
static idx_t  cached_max = 0;  /* largest n already computed */
static size_t cache_cap   = 0;  /* allocated capacity */

/* Ensure that cache contains values up to at least n. */
static void
ensure_cache(idx_t n)
{
    /* Initialise cache with the first three values if it is empty. */
    if (cache == NULL) {
        cache_cap   = 8;     /* start with some spare space */
        cache       = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = 1;        /* a(0) */
        cache[1] = 1;        /* a(1) */
        cache[2] = 3;        /* a(2) */
        cached_max = 2;
    }

    /* Grow the cache if needed. */
    if (n > cached_max) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;

        /* Compute new values using the recurrence. */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            /* a(i) = 3*a(i-1) - a(i-2) - 2*a(i-3) */
            cache[i] = 3 * cache[i - 1]
                     -     cache[i - 2]
                     - 2 * cache[i - 3];
        }
        cached_max = n;
    }
}

seq_t
a(idx_t n)
{
    if (n == 0) return 1;  /* protect very small n before cache init */

    ensure_cache(n);
    return cache[n];
}
