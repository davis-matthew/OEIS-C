/*
 * A190959  a(n) = 3*a(n-1) - 5*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-1) - 5*a(n-2), a(0)=0, a(1)=1
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 27, i64 n <= 54, i128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache the previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cap = 0;

    /* base cases, works for any implementation of seq_t */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* first call: allocate storage and compute up to n */
    if (!cache) {
        size_t sz = (size_t)(n + 1);
        cache = (seq_t*)malloc(sz * sizeof(seq_t));
        if (!cache) return 0;               /* fallback on allocation failure */
        cache[0] = 0;
        cache[1] = 1;
        for (idx_t i = 2; i <= n; ++i) {
            cache[i] = 3 * cache[i-1] - 5 * cache[i-2];
        }
        cap = sz;
        return cache[n];
    }

    /* enlarge cache if a larger n has been requested */
    if (cap <= n) {
        size_t sz = (size_t)(n + 1);
        seq_t* tmp = (seq_t*)realloc(cache, sz * sizeof(seq_t));
        if (!tmp) return 0;                 /* fallback on allocation failure */
        cache = tmp;
        for (idx_t i = cap; i <= n; ++i) {
            cache[i] = 3 * cache[i-1] - 5 * cache[i-2];
        }
        cap = sz;
    }

    return cache[n];
}
