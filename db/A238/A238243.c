/*
 * A238243  A recursive sequence: a(n) = Fibonacci(n)*a(n-1) + 2.
 * https://oeis.org/A238243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n) * a(n-1) + 2 , a(1) = 1 (recurrence)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0. */
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache = NULL;     /* cache[i] = a(i) */
    static size_t cache_cap = 0;    /* current capacity of cache */
    static idx_t cache_max = 0;     /* maximum index with valid value */
    static bool initialized = false;

    /* Initialise the cache for the first term. */
    if (!initialized) {
        /* Ensure that the cache can hold index 1. */
        ensure_cache(1, &cache, &cache_cap);
        cache[1] = (seq_t)1;   /* a(1) = 1 */
        cache_max = 1;
        initialized = true;
    }

    /* Expand cache if necessary. */
    if (n > cache_max) {
        ensure_cache(n, &cache, &cache_cap);

        for (idx_t k = cache_max + 1; k <= n; ++k) {
            /* a(k) = Fib(k) * a(k-1) + 2 */
            seq_t fk   = fib(k);          /* Fibonacci(k) */
            seq_t prev = cache[k - 1];    /* a(k-1) */
            seq_t val  = fk * prev + (seq_t)2;
            cache[k] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
