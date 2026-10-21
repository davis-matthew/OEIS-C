/*
 * A063290  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          17 ).
 * https://oeis.org/A063290
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 13, i16 n <= 52, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A063290: a(n) = a(n-4) + a(n-6) - a(n-10)   (for n > 13)
   Offset: 2
   Initial values:
     a(2)= -1, a(3)= 5, a(4)= 16, a(5)= 28, a(6)= 40, a(7)= 52,
     a(8)= 64, a(9)= 76, a(10)=88, a(11)=100, a(12)=112, a(13)=122
*/
#include <stdlib.h>

static const seq_t init_vals[14] = {
    0, 0,          /* indices 0,1 are unused */
    -1, 5, 16, 28, /* 2-5 */
    40, 52, 64, 76,/* 6-9 */
    88, 100, 112, 122 /* 10-13 */
};

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* defined behavior only for n ≥ 2 */

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t current_max = 1;   /* highest n already computed in cache */

    /* Ensure the cache can hold up to n */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise base values up to 13 if not yet done */
    if (current_max < 13) {
        if (cache_size <= 13) {            /* safe reallocation if needed */
            seq_t *tmp = (seq_t*)realloc(cache, 14 * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            cache = tmp;
            cache_size = 14;
        }
        for (idx_t i = 2; i <= 13; ++i)
            cache[i] = init_vals[i];
        current_max = 13;
    }

    /* Compute missing terms up to requested n */
    for (idx_t i = current_max + 1; i <= n; ++i) {
        /* recurrence requires i-4, i-6, i-10 to be valid */
        seq_t v1 = cache[i-4];
        seq_t v2 = cache[i-6];
        seq_t v3 = cache[i-10];
        cache[i] = v1 + v2 - v3;
    }
    current_max = n;
    return cache[n];
}
