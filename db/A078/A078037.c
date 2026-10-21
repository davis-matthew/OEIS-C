/*
 * A078037  Expansion of (1-x)/(1+2*x^2+2*x^3).
 * https://oeis.org/A078037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, * a(1) = -1, * a(2) = -2, * a(n) = -2*a(n-2)-2*a(n-3)
 *            for n >= 3 * (Recurrence derived from (1-x)/(1+2*x^2+2*x^3))
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 46, i64 n <= 93, i128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoised values */
    static seq_t *cache = NULL;    /* array of already computed terms   */
    static size_t cache_sz = 0;    /* allocated size                    */
    static idx_t max_comp = 0;     /* highest index that has been computed (inclusive) */

    /* grow the cache if necessary */
    size_t need = (size_t)n + 1;
    if (need > cache_sz) {
        size_t new_sz = cache_sz ? cache_sz * 2 : 4;
        if (new_sz < need) new_sz = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure → return 0 */
        cache = tmp;
        cache_sz = new_sz;
    }

    /* initialise the first three terms on the first call */
    if (max_comp == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)-2;
        max_comp = 2;
    }

    /* compute any missing terms up to n */
    for (size_t i = (size_t)max_comp + 1; i < need; ++i) {
        /* recurrence for n >= 3; use promoted arithmetic on the right‑hand side */
        seq_t t1 = cache[i - 2];
        seq_t t2 = cache[i - 3];
        cache[i] = -((seq_t)2) * t1 - ((seq_t)2) * t2;
    }
    if (n > max_comp) max_comp = n;

    return cache[n];
}
