/*
 * A041343  Denominators of continued fraction convergents to sqrt(185).
 * https://oeis.org/A041343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 24, u64 n <= 46, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: A041343
 * Denominators of continued fraction convergents to sqrt(185).
 *
 * Recurrence:  a(n) = 136 * a(n-5) + a(n-10)   for n >= 10
 * Initial values:
 *   a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=5,
 *   a(5)=133, a(6)=138, a(7)=271, a(8)=409, a(9)=680
 *
 * This implementation caches computed terms in a static array.
 * Calls can be in any order; the cache grows as needed.
 */

#include <stdlib.h>

/* The function's external linkage as required */
seq_t a(idx_t n) {
    /* Static cache of computed terms and its current capacity (number of valid entries) */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;       /* size of allocated array */

    /* First 10 terms explicitly defined */
    static const seq_t init[10] = {
        (seq_t)1,
        (seq_t)1,
        (seq_t)2,
        (seq_t)3,
        (seq_t)5,
        (seq_t)133,
        (seq_t)138,
        (seq_t)271,
        (seq_t)409,
        (seq_t)680
    };

    /* Preserve old capacity to know from where to start filling */
    size_t old_cap = cache_cap;

    /* Grow cache if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;                /* we need indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;                          /* allocation failure: return 0 as safe fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to and including n */
    for (idx_t i = (idx_t)old_cap; i <= n; ++i) {
        if (i < 10) {
            cache[i] = init[i];
        } else {
            /* a(i) = 136 * a(i-5) + a(i-10) */
            seq_t term = cache[i - 5] * (seq_t)136;
            term = term + cache[i - 10];
            cache[i] = term;
        }
    }

    return cache[n];
}
