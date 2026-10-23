/*
 * A144471  Inverse binomial transform of A020806.
 * https://oeis.org/A144471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=3, a(2)=-5, a(3)=13, a(n) = -3*a(n-1) - 3*a(n-2) -
 *            2*a(n-3) for n>3
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t  cap = 0;     /* allocated size */
    static size_t  computed = 0;/* number of terms actually computed */
    size_t idx = (size_t)n;    /* assume n fits in size_t for the tests */

    /* initial allocation and first four terms */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;              // out of memory
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)-5;
        cache[3] = (seq_t)13;
        computed = 4;
    }

    /* grow allocation if necessary */
    if (idx + 1 > cap) {
        size_t newcap = cap;
        while (newcap <= idx) newcap <<= 1;            /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;                          /* out of memory */
        cache = tmp;
        cap = newcap;
    }

    /* compute any missing terms up to idx */
    while (computed <= idx) {
        size_t i = computed;
        cache[i] = (seq_t)(-3)*cache[i-1] + (seq_t)(-3)*cache[i-2] + (seq_t)(-2)*cache[i-3];
        ++computed;
    }

    return cache[idx];
}
