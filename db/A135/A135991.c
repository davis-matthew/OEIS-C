/*
 * A135991  Expansion of x^3*(x-1)^2*(x+1) / (x^6-3*x^5+3*x^4-x+1).
 * https://oeis.org/A135991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 3*a(n-4) + 3*a(n-5) - a(n-6) for n ≥ 7, with
 *            initial values a(1) = 0, a(2) = 0, a(3) = 1, a(4) = 0, a(5) =
 *            –1, a(6) = 0
 * Range:     i8 n <= 22, i16 n <= 42, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* array holding a(0) … a(max_n) */
    static size_t cache_size = 0; /* number of allocated elements */

    /* first call: allocate space for indices 0 … 6 (seven elements) */
    if (cache == NULL) {
        cache_size = 7;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;           /* unused dummy for index 0 */
        cache[1] = (seq_t)0;           /* a(1) */
        cache[2] = (seq_t)0;           /* a(2) */
        cache[3] = (seq_t)1;           /* a(3) */
        cache[4] = (seq_t)0;           /* a(4) */
        cache[5] = (seq_t)-1;          /* a(5) */
        cache[6] = (seq_t)0;           /* a(6) */
    }

    /* if we already have the requested value, return it */
    if ((size_t)n < cache_size)
        return cache[n];

    /* we need to grow the cache to hold indices up to n */
    size_t old_size = cache_size;
    cache_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    if (!tmp) {
        /* on allocation failure we abort – caller can never recover from
           an OOM in this context, but this keeps the implementation simple. */
        abort();
    }
    cache = tmp;

    /* compute a(old_size) … a(n) using the recurrence */
    for (size_t i = old_size; i <= (size_t)n; ++i) {
        /* a(i) = a(i-1) - 3*a(i-4) + 3*a(i-5) - a(i-6) */
        seq_t val = cache[i - 1]
                  - (seq_t)3 * cache[i - 4]
                  + (seq_t)3 * cache[i - 5]
                  - cache[i - 6];
        cache[i] = val;
    }

    return cache[n];
}
