/*
 * A153977  One-fourth of partial sums of A153976.
 * https://oeis.org/A153977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (triangular(n-1)^2 + triangular(n+1)^2 - 1)/4 Recurrence:
 *            (n-1)*(n^2 - n + 6)*a(n) = (n+1)*(n^2 + n + 6)*a(n-1), a(1)=2
 *            (The recurrence is valid for all n >= 2.) The function uses
 *            memoisation to handle calls in any order
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 430, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Cache of already computed values: cache[0] = a(1), cache[1] = a(2), ... */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* allocated size of cache */
    static size_t computed = 0;   /* number of terms actually computed (1..computed) */

    if (n == 0) return 0; /* Undefined for n=0, but return 0 safely */

    /* Ensure cache has enough capacity for n terms */
    if (capacity < (size_t)n) {
        size_t new_cap = (capacity < 16) ? 16 : capacity * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed: abort */
            abort();
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialise first term if not already done */
    if (computed == 0) {
        cache[0] = (seq_t)2;   /* a(1) = 2 */
        computed = 1;
    }

    /* Compute terms up to n if necessary */
    for (size_t k = computed + 1; k <= (size_t)n; ++k) {
        idx_t i = (idx_t)k;                        /* current index (1-based) */
        seq_t prev = cache[k - 2];                 /* a(i-1) */
        __uint128_t a_prev = (__uint128_t)prev;

        /* numerator = (i+1) * (i^2 + i + 6) */
        __uint128_t i128 = (__uint128_t)i;
        __uint128_t num = (__uint128_t)(i + 1) * (i128 * i128 + i128 + 6);

        /* denominator = (i-1) * (i^2 - i + 6) */
        __uint128_t den = (__uint128_t)(i - 1) * (i128 * i128 - i128 + 6);

        /* a(i) = a_prev * num / den ; the division is exact */
        __uint128_t val = a_prev * num / den;
        cache[k - 1] = (seq_t)val;
    }

    computed = (size_t)n;       /* mark that terms through n are computed */
    return cache[n - 1];
}
