/*
 * A177825  Expansion of 1/((1 + x^3 - x^4)*(1 - x)).
 * https://oeis.org/A177825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - a(n-3) + 2*a(n-4) - a(n-5) Initial values:
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=0, a(4)=1, a(5)=1
 * Range:     i8 n <= 33, i16 n <= 61, i32 n <= 116, i64 n <= 228,
 *            i128 n <= 450, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static const seq_t init[6] = {
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)0,
        (seq_t)1, (seq_t)1
    };

    static seq_t *cache   = NULL;
    static idx_t  cap     = 0;     /* capacity of allocated array (in entries) */
    static idx_t  max_used = 0;    /* highest index that has been computed */

    /* If n is within the known initial block, return it directly. */
    if (n < 6) {
        return init[(size_t)n];
    }

    /* Ensure the cache is large enough for indices up to n. */
    if (cache == NULL) {
        /* First allocation: allocate enough for n+1 entries and copy
           the initial 6 values. */
        cap = n + 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */
        for (idx_t i = 0; i < 6; ++i) {
            cache[i] = init[i];
        }
        max_used = 5;                         /* indices 0..5 computed */
    } else if (cap <= n) {
        /* Grow the cache to accommodate index n. */
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* reallocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute any missing entries from max_used+1 up to n. */
    for (idx_t i = max_used + 1; i <= n; ++i) {
        cache[i] = cache[i-1]
                 - cache[i-3]
                 + (seq_t)2 * cache[i-4]
                 - cache[i-5];
    }
    max_used = n;

    return cache[n];
}
