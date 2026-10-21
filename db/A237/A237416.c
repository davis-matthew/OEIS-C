/*
 * A237416  Smallest multiple of 5 beginning with n.
 * https://oeis.org/A237416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-5) - a(n-10) for n >= 11 Initial values: a(1)=10,
 *            a(2)=20, a(3)=30, a(4)=40, a(5)=5, a(6)=60, a(7)=70, a(8)=80,
 *            a(9)=90, a(10)=10
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Base values for indices 1..10.  Index 0 is unused. */
static const seq_t _base_vals[11] = {0, 10, 20, 30, 40, 5,
                                      60, 70, 80, 90, 10};

/* Cache for computed values.  cache[0] is unused. */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* largest index computed (>=0) */

/* Ensure that cache holds values up to at least n. */
static void _ensure_cache(idx_t n)
{
    size_t target = (size_t)n;
    if (target <= cache_size) return;

    size_t new_cap = target + 1;
    seq_t *new_cache;

    if (cache == NULL) {
        new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        /* Initialize available base values. */
        for (size_t i = 1; i <= 10 && i < new_cap; ++i)
            new_cache[i] = _base_vals[i];
    } else {
        new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
    }
    cache = new_cache;

    /* Compute missing entries. */
    for (size_t i = cache_size + 1; i <= target; ++i) {
        if (i <= 10) {
            cache[i] = _base_vals[i];
        } else {
            seq_t tmp = cache[i - 5] + cache[i - 5];
            cache[i] = tmp - cache[i - 10];
        }
    }
    cache_size = target;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, but never used */
    _ensure_cache(n);
    return cache[(size_t)n];
}
