/*
 * A097949  G.f.: -(2+7*x-x^3)/(1+4*x-4*x^3-x^4).
 * https://oeis.org/A097949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -(-1)^n - (1/6)*( ( -sqrt(3) - 2)^n + ( sqrt(3) - 2)^n )
 *            - 2/3
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 17, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent linear recurrence: a(n) = -4*a(n-1) + 4*a(n-3) + a(n-4)  for n>=4 */
/* Initial values: a(0)=-2, a(1)=1, a(2)=-4, a(3)=9 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;     /* capacity of the cache */
    static size_t len = 0;     /* number of elements currently stored (always <= cap) */

    /* initialise cache if needed */
    if (cache == NULL) {
        /* reserve space for the first four values */
        cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (cache == NULL) return (seq_t)0;          /* out of memory – unlikely in tests */
        /* store initial values */
        cache[0] = (seq_t)(-2);
        cache[1] = (seq_t)(1);
        cache[2] = (seq_t)(-4);
        cache[3] = (seq_t)(9);
        len = 4;
    }

    /* extend cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap * 2;
        if (newcap < (size_t)n + 1) newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        if (tmp == NULL) return (seq_t)0;  /* out of memory – unlikely */
        cache = tmp;
        cap = newcap;
    }

    /* compute terms up to n if not already present */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        if (i < 4) continue;   /* initial values already set */
        /* recurrence: a(i) = -4*a(i-1) + 4*a(i-3) + a(i-4) */
        seq_t term = -( (seq_t)4 * cache[i-1] );
        term += ( (seq_t)4 * cache[i-3] );
        term += cache[i-4];
        cache[i] = term;
    }

    len = (size_t)n + 1;   /* we now have all values up to n */
    return cache[n];
}
