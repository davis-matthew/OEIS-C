/*
 * A017892  Expansion of 1/(1 - x^10 - x^11 - x^12 - x^13 - x^14 - x^15 -
 *          x^16).
 * https://oeis.org/A017892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 88, u32 n <= 161, u64 n <= 305,
 *            u128 n <= 595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-10)+a(n-11)+a(n-12)+a(n-13)+a(n-14)+a(n-15)+a(n-16);
   Initial values: a(0)=1, a(1..9)=0, a(10..15)=1. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* number of valid entries in cache */

    /* initialise base cases once */
    if (cache == NULL) {
        cap = 16;                     /* we will hold indexes 0 … 15 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failed */

        /* a(0) */
        cache[0] = (seq_t)1;
        /* a(1) … a(9) */
        for (idx_t i = 1; i <= 9; ++i) cache[i] = (seq_t)0;
        /* a(10) … a(15) */
        for (idx_t i = 10; i <= 15; ++i) cache[i] = (seq_t)1;
        /* a(16) will be produced by the recurrence when needed */
    }

    /* enlarge the cache if a larger n is requested */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;     /* allocation failed */
        cache = tmp;

        /* compute new entries using the recurrence */
        for (idx_t i = (idx_t)cap; i <= n; ++i) {
            seq_t sum = cache[i-10] + cache[i-11] + cache[i-12] +
                        cache[i-13] + cache[i-14] + cache[i-15] + cache[i-16];
            cache[i] = sum;
        }
        cap = new_cap;
    }

    return cache[n];
}
