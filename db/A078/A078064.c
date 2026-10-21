/*
 * A078064  Expansion of (1-x)/(1+2*x+x^2-x^3).
 * https://oeis.org/A078064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 27, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=-3, a(2)=5,
   a(n) = -2*a(n-1)-a(n-2)+a(n-3)  for n>2 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoized values */
    static idx_t size   = 0;      /* allocated size of cache */

    /* Ensure the cache covers indices up to n */
    if (n >= size) {
        idx_t newsize = (n + 1 > 3) ? (n + 1) : 3;  /* at least 3 elements for the base */
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) return 0;        /* out of memory; return 0 as a fallback */
        cache = tmp;

        /* Initialise the first three terms when the cache is empty */
        if (size == 0) {
            cache[0] = (seq_t) 1;
            cache[1] = (seq_t)-3;
            cache[2] = (seq_t) 5;
            size = 3;
        }

        /* Recursively fill new entries */
        for (idx_t i = size; i < newsize; ++i) {
            cache[i] = -2 * cache[i-1] - cache[i-2] + cache[i-3];
        }
        size = newsize;
    }
    return cache[n];
}
