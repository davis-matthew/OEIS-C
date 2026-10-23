/*
 * A143392  A quadratic recursion sequence: a(n)=a(n - 1)^2 - 2*a(n - 1) - a(n
 *          - 2)^2 + 2*a(n - 2).
 * https://oeis.org/A143392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1)^2 - 2*a(n-1) - a(n-2)^2 + 2*a(n-2); Initial
 *            values: a(1) = 1, a(2) = 2
 * Range:     i8 n <= 8, i16 n <= 9, i32 n <= 10, i64 n <= 11, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static void ensure_cache(idx_t n) {
    if (n <= cache_size)
        return;

    /* Grow the cache array */
    idx_t new_cap = (cache_size == 0) ? 2 : cache_size * 2;
    if (new_cap < n)
        new_cap = n;
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – abort; the caller will observe failure */
        abort();
    }
    cache = tmp;

    /* Compute new sequence values */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)1;
        } else if (i == 2) {
            val = (seq_t)2;
        } else {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            val = a1 * a1 - 2 * a1 - a2 * a2 + 2 * a2;
        }
        cache[i] = val;
    }
    cache_size = n;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;      /* Undefined for n=0, return 0 as a safe default */

    ensure_cache(n);
    return cache[n];
}
