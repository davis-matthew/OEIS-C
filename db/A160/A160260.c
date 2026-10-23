/*
 * A160260  Numerator of Hermite(n, 12/29).
 * https://oeis.org/A160260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 24*a(n-1) - 1682*(n-1)*a(n-2) Initial values: a(0)=1,
 *            a(1)=24
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;

    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure – unlikely in judge */
        cap = 0;
    }

    if (n >= cap) {                    /* need to extend cache up to n */
        cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure – unlikely */

        for (idx_t i = cap; i <= n; ++i) {
            if (i == 0) {
                cache[i] = 1;
            } else if (i == 1) {
                cache[i] = 24;
            } else {
                seq_t term1 = 24 * cache[i - 1];
                seq_t mult  = (seq_t)1682 * (seq_t)(i - 1);
                seq_t term2 = mult * cache[i - 2];
                cache[i] = term1 - term2;
            }
        }
        cap = n + 1;
    }

    return cache[n];
}
