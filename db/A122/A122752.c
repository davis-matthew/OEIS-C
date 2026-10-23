/*
 * A122752  a(0) = 1; a(1) = 1; a(2) = 1; a(n) = (n-1)*a(n-1) + (n-2)*a(n-2) +
 *          (n-3)*a(n-3) for n >= 3.
 * https://oeis.org/A122752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=a(1)=a(2)=1; a(n) = (n-1)*a(n-1)+(n-2)*a(n-2)+(n-3)*a(n-3) for n>=3 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed_upto = 0;   /* first index not yet computed */

    /* Ensure the cache can hold indices 0..n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n */
    for (idx_t i = computed_upto; i <= n; ++i) {
        if (i < 3) {
            cache[i] = (seq_t)1;
        } else {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];

            seq_t c1 = (seq_t)(i - 1);
            seq_t c2 = (seq_t)(i - 2);
            seq_t c3 = (seq_t)(i - 3);

            cache[i] = c1 * a1 + c2 * a2 + c3 * a3;
        }
    }

    computed_upto = n + 1;
    return cache[n];
}
