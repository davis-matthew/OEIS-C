/*
 * A077991  Expansion of 1/(1+2*x+2*x^2-2*x^3).
 * https://oeis.org/A077991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) + 2*a(n-1) + 2*a(n-2) - 2*a(n-3) = 0, with a(0)=1, a(1)=-2,
 *            a(2)=2.Equivalent: a(n)= -2*a(n-1)-2*a(n-2)+2*a(n-3)
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 34, i64 n <= 70, i128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;          /* previously computed terms */
    static size_t cached = 0;            /* number of terms in cache (size = cached) */

    if (n >= cached) {
        size_t newsize = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);

        for (size_t i = cached; i < newsize; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)-2;
            } else if (i == 2) {
                cache[i] = (seq_t)2;
            } else {
                /* a(n) = -2*a(n-1) - 2*a(n-2) + 2*a(n-3) */
                seq_t term = - ((seq_t)2 * cache[i - 1]) 
                            - ((seq_t)2 * cache[i - 2]) 
                            + ((seq_t)2 * cache[i - 3]);
                cache[i] = term;
            }
        }
        cached = newsize;
    }
    return cache[n];
}
