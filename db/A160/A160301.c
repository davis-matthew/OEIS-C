/*
 * A160301  Numerator of Hermite(n, 3/31).
 * https://oeis.org/A160301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 31^n * Hermite(n, 3/31). Recurrence: a(0)=1, a(1)=6,
 *            a(n+2)=6*a(n+1)-1922*(n+1)*a(n)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Ensure cache can hold indices 0 and 1 */
    if (cache_cap < 2) {
        size_t new_cap = 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize base values if not yet set */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len == 1) {
        cache[1] = (seq_t)6;
        cache_len = 2;
    }

    if (n < cache_len) {
        return cache[n];
    }

    /* Resize cache if necessary */
    size_t need = n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term1 = cache[i - 1] * (seq_t)6;
        seq_t multiplier = (seq_t)1922 * (seq_t)(i - 1);
        seq_t term2 = cache[i - 2] * multiplier;
        cache[i] = term1 - term2;
    }

    cache_len = n + 1;
    return cache[n];
}
