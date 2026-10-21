/*
 * A097948  G.f.: -(1-3*x^2-x^3)/(1+4*x-4*x^3-x^4).
 * https://oeis.org/A097948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = –1, a(1) = 4, a(2) = –13, a(3) = 49 and for n ≥ 4: a(n) =
 *            –4*a(n-1) + 4*a(n-3) + a(n-4)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 16, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated capacity */
    static idx_t len   = 0;   /* number of elements computed so far */

    /* initialise cache with the first four terms */
    if (cap == 0) {
        cap = 4;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* out of memory – return 0 */
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)-13;
        cache[3] = (seq_t)49;
        len = 4;
    }

    /* if n is within the cached range, return directly */
    if (n < len)
        return cache[n];

    /* grow the cache until it can hold n+1 elements */
    if (n + 1 > cap) {
        idx_t new_cap = cap;
        while (new_cap < n + 1)
            new_cap = new_cap * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            cap = new_cap;
        } else {
            /* allocation failed – return 0 */
            return 0;
        }
    }

    /* compute terms from len up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t term1 = - ((seq_t)4 * cache[i-1]);   /* –4*a(i-1) */
        seq_t term2 =   ((seq_t)4 * cache[i-3]);   /* +4*a(i-3) */
        seq_t term3 =   cache[i-4];                /* +a(i-4)   */
        cache[i] = term1 + term2 + term3;
    }
    len = n + 1;
    return cache[n];
}
