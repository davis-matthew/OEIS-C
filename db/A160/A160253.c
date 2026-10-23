/*
 * A160253  Numerator of Hermite(n, 10/29).
 * https://oeis.org/A160253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29^n * Hermite(n, 10/29) Recurrence: a(0) = 1, a(1) = 20,
 *            a(n+1) = 20*a(n) - 1682*n*a(n-1) for n ≥ 1
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;   /* highest n that has been computed */
    static size_t cap = 0;       /* current allocated capacity */

    if (n == 0) return (seq_t)1;

    /* if value already computed, return it */
    if (cached_n >= 1 && n <= cached_n)
        return cache[(size_t)n];

    /* First call: allocate cache and fill base values */
    if (cache == NULL) {
        cap = (size_t)(n + 1);
        if (cap < 2) cap = 2;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (cache == NULL) abort();   /* out of memory */
        cache[0] = (seq_t)1;
        if (cap > 1) cache[1] = (seq_t)20;
        cached_n = 1;
    }
    /* enlarge the cache if needed */
    else if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < cap * 2) new_cap = cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();    /* out of memory */
        /* zero the newly allocated part */
        memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;
    }

    /* compute all missing values up to n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        /* a(i) = 20*a(i-1) - 1682*(i-1)*a(i-2)  */
        seq_t term1 = (seq_t)20 * cache[(size_t)(i - 1)];
        seq_t term2 = (seq_t)1682 * (seq_t)(i - 1) * cache[(size_t)(i - 2)];
        cache[(size_t)i] = term1 - term2;
    }
    cached_n = n;
    return cache[(size_t)n];
}
