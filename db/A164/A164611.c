/*
 * A164611  Expansion of (1 + x + 2*x^2 - x^3)/(1 - 2*x + 3*x^2 - 2*x^3 +
 *          x^4).
 * https://oeis.org/A164611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1) - 3*a(n-2) + 2*a(n-3) - a(n-4), with a(0)=1,
 *            a(1)=3, a(2)=5, a(3)=2
 * Range:     i8 n <= 64, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 0) return 1;
    if (n == 1) return 3;
    if (n == 2) return 5;
    if (n == 3) return 2;

    /* Static cache for previously computed values. */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;   /* number of values stored in cache */

    /* Allocate cache for the initial four terms on first call. */
    if (cache == NULL) {
        cache = (seq_t*)malloc(4 * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 3;
        cache[2] = 5;
        cache[3] = 2;
        cached_len = 4;
    }

    /* Return cached value if already computed. */
    if (n < cached_len) return cache[n];

    /* Extend cache up to n. */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_len * sizeof(seq_t));
    if (tmp == NULL) {
        /* Allocation failure – return 0 as a fallback (unlikely in tests). */
        return 0;
    }
    cache = tmp;

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cached_len; i <= n; ++i) {
        cache[i] = 2 * cache[i-1] - 3 * cache[i-2] + 2 * cache[i-3] - cache[i-4];
    }
    cached_len = new_len;

    return cache[n];
}
