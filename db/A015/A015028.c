/*
 * A015028  q-factorial numbers for q=-13.
 * https://oeis.org/A015028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Product_{k=1..n} ((-13)^k - 1) / (-13 - 1) Equivalent
 *            recurrence: a(0) = 1 a(n) = ((-13)^n - 1) * a(n-1) / (-14) for n
 *            ≥ 1 The function memoises the sequence so calls may come in any
 *            order
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 4, i64 n <= 6, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence generator for OEIS A015028 */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* stores a(0) .. a(current_max)   */
    static size_t cache_len = 0;    /* number of entries stored (max index+1) */
    static size_t cache_cap = 0;    /* current capacity of cache          */

    /* power holds (-13)^k, where k == cache_len (i.e. next exponent to use). */
    static seq_t power = 0;
    static idx_t power_k = 0;

    /* Ensure the cache is large enough for the requested index. */
    if (n >= cache_len) {
        /* Initialisation if this is the first call. */
        if (cache_len == 0) {
            cache_cap = 1;
            cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
            cache[0] = 1;          /* a(0) = 1 */
            cache_len = 1;
            power  = -13;          /* (-13)^1 for next iteration   */
            power_k = 1;
        }

        /* Enlarge capacity if necessary. */
        if ((size_t)n >= cache_cap) {
            size_t new_cap = cache_cap;
            if (new_cap == 0) new_cap = 1;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            void *tmp = realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failed: abort (no further graceful handling). */
                abort();
            }
            cache = (seq_t *)tmp;
            cache_cap = new_cap;
        }

        /* Compute values from cache_len up to n inclusively. */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* term = (-13)^i - 1  (power currently holds (-13)^i) */
            seq_t term = power - 1;
            /* a(i) = term * a(i-1) / (-14) */
            seq_t val  = cache[i - 1];
            val = val * term;
            val = val / (seq_t)-14;
            cache[i] = val;
            /* Update power for next exponent. */
            power = power * (seq_t)-13;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
