/*
 * A078043  Expansion of (1 - x)/(1 + x - x^2 + 2*x^3).
 * https://oeis.org/A078043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A078043
   Recurrence: a(0)=1, a(1)=-2, a(2)=3, a(n) = -a(n-1) + a(n-2) - 2*a(n-3)  */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated size             */
    static idx_t len   = 0;   /* number of values stored    */

    /* initialise the cache lazily with the first 3 terms */
    if (len == 0) {
        cap = 4;                     /* start with a bit more than 3 */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-2;
        cache[2] = (seq_t)3;
        len = 3;
    }

    /* if already computed, return directly */
    if (n < len) {
        return cache[n];
    }

    /* grow cache enough to hold up to n */
    if ((idx_t)(n + 1) > cap) {
        idx_t new_cap = cap;
        while ((idx_t)(n + 1) > new_cap) new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute terms from current length up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t val;
        if (i == 0)        val = (seq_t)1;
        else if (i == 1)   val = (seq_t)-2;
        else if (i == 2)   val = (seq_t)3;
        else               val = -cache[i-1] + cache[i-2] - (seq_t)2 * cache[i-3];
        cache[i] = val;
    }
    len = n + 1;
    return cache[n];
}
