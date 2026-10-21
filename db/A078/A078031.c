/*
 * A078031  Expansion of (1-x)/(1 + x^2 - x^3).
 * https://oeis.org/A078031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-2) + a(n-3); Initial values: a(0)=1, a(1)=-1,
 *            a(2)=-1
 * Range:     i8 n <= 24, i16 n <= 53, i32 n <= 112, i64 n <= 229,
 *            i128 n <= 459, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   // for malloc, realloc
#include <stddef.h>   // for size_t

static seq_t *cache = NULL;       // memoization array
static idx_t cache_len = 0;       // number of values currently stored (cache_len-1 is max index)
static idx_t cache_cap = 0;       // allocated capacity

/* Computes a(n) for A078031 using recurrence a(n) = -a(n-2)+a(n-3) */
seq_t a(idx_t n)
{
    /* Ensure the cache holds values up to n */
    if (n + 1 > cache_cap) {
        /* Grow the cache: double capacity or reach n+1, whichever is larger */
        idx_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 4;
        if (new_cap < n + 1) new_cap = n + 1;

        /* realloc uses size_t; cast is safe for realistic n */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – abort */
            abort();
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 1;
        } else if (i == 1) {
            val = -1;
        } else if (i == 2) {
            val = -1;
        } else {
            /* recurrence: a(i) = -a(i-2) + a(i-3) */
            val = -cache[i - 2] + cache[i - 3];
        }
        cache[i] = val;
    }

    cache_len = n + 1;   /* record that indices [0..n] are valid */
    return cache[n];
}
