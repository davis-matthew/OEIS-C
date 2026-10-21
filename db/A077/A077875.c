/*
 * A077875  Expansion of (1-x)^(-1)/(1-x+2*x^2-x^3).
 * https://oeis.org/A077875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   linear recurrence from G(x)=1/(1-2x+3x^2-3x^3+x^4)
 * Range:     i8 n <= 18, i16 n <= 38, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 2*a(n-1) - 3*a(n-2) + 3*a(n-3) - a(n-4),  n ≥ 4   */
/* initial values: a(0)=1, a(1)=2, a(2)=1, a(3)=-1 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;          /* cache of computed terms   */
    static size_t cache_len = 0;         /* number of terms stored    */
    static size_t cache_cap = 0;         /* allocated capacity        */

    /* Grow cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base values once only */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)(-1);
        cache_len = 4;
    }

    /* If requested index already computed, return it */
    if (n < cache_len)
        return cache[n];

    /* Compute missing terms using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t t = ((seq_t)2 * cache[i - 1])   /* 2*a(i-1)  */
                 - ((seq_t)3 * cache[i - 2])   /* -3*a(i-2) */
                 + ((seq_t)3 * cache[i - 3])   /* +3*a(i-3) */
                 - cache[i - 4];               /* -a(i-4)   */
        cache[i] = t;
    }
    cache_len = n + 1;
    return cache[(size_t)n];
}
