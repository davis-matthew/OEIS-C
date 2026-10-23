/*
 * A106541  a(n) = a(n-1) - 2*a(n-2) - 3*a(n-3) - ... - (n-1)*a(1), with a(1)
 *          = a(2) = 2, a(3) = -2.
 * https://oeis.org/A106541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 3*a(n-1) - 5*a(n-2) + 2*a(n-3), n > 3 Initial values:
 *            a(1) = 2, a(2) = 2, a(3) = -2 (This is equivalent to a(n) =
 *            a(n-1) - Σ_{k=2}^{n-1} k*a(n-k))
 * Range:     i8 n <= 7, i16 n <= 17, i32 n <= 33, i64 n <= 68, i128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
seq_t a(idx_t n) {
    if (n == 1 || n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)-2;

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed_to = 0;

    size_t sz = (size_t)n + 1;
    if (sz > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, sz * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = sz;
    }

    if (computed_to == 0) {
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)-2;
        computed_to = 3;
    }

    while (computed_to < n) {
        idx_t k = computed_to + 1;
        seq_t val = (seq_t)3 * cache[k - 1] - (seq_t)5 * cache[k - 2] + (seq_t)2 * cache[k - 3];
        cache[k] = val;
        computed_to = k;
    }

    return cache[n];
}
