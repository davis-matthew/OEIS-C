/*
 * A163606  a(n) = ((3 + 2*sqrt(2))*(3 + sqrt(2))^n + (3 - 2*sqrt(2))*(3 -
 *          sqrt(2))^n)/2.
 * https://oeis.org/A163606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2), a(0)=3, a(1)=13
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* size allocated in cache */
    static idx_t max_cached = 0;    /* highest index currently stored */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* allocate space for at least two elements */
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* base values */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)13;
        max_cached = 1;
    }

    /* grow cache if needed */
    while (n > (idx_t)(cap - 1)) {
        /* double the size, but at least one more slot */
        size_t new_cap = cap < 2 ? 2 : cap * 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t v1 = cache[i - 1] * (seq_t)6;
        seq_t v2 = cache[i - 2] * (seq_t)7;
        cache[i] = v1 - v2;      /* subtraction is safe: 6*a[n-1] > 7*a[n-2] for n>=2 */
    }
    max_cached = n > max_cached ? n : max_cached;

    return cache[n];
}
