/*
 * A190609  a(1)=a(2)=1; thereafter a(n) = 3*(a(ceiling(n/2))-a(floor(n/2))).
 * https://oeis.org/A190609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=a(2)=1; for n>=3, a(n) = 3*(a(ceil(n/2)) - a(floor(n/2)))
 * Range:     i8 n <= 64, i16 n <= 88, i32 n <= 88, i64 n <= 88, i128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t computed = 2;   /* highest index whose value has been computed */

    /* Initial allocation and base values */
    if (cache == NULL) {
        cap = 4;                                  /* indices 0..3 */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
        /* cache[3] will be computed on first use */
    }

    /* Return immediately if already computed */
    if (n <= computed) {
        return cache[n];
    }

    /* Ensure sufficient storage */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t newcap = cap;
        while (newcap < need) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure, abort safely */
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing values incrementally */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i < 3) continue;            /* guards against any future changes */
        idx_t ceil = (i + 1) >> 1;      /* ceil(i/2) */
        idx_t floor = i >> 1;           /* floor(i/2) */
        cache[i] = (seq_t)3 * (cache[ceil] - cache[floor]);
    }
    computed = n;
    return cache[n];
}
