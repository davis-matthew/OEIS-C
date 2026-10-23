/*
 * A014024  Inverse of 15th cyclotomic polynomial.
 * https://oeis.org/A014024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)-a(n-3)+a(n-4)-a(n-5)+a(n-7)-a(n-8),
   a(0)=1, a(1)=1, a(2)=1, a(3)=0, a(4)=0, a(5)=-1, a(6)=-1, a(7)=-1, a(8)=0;
   Periodic with period 15. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache holds the sequence values up to the largest index
       that has been computed. */
    static seq_t *cache = NULL;          /* array of computed terms   */
    static size_t  cap   = 0;           /* allocated array size      */
    static idx_t   max_n = (idx_t)-1;    /* largest computed index    */

    /* First call – create the cache and set the initial values. */
    if (cache == NULL) {
        /* Allocate a safe initial capacity (at least 9 for indices 0..8). */
        size_t init = 16;
        if (init < 9) init = 9;
        cache = (seq_t*)malloc(init * sizeof(seq_t));
        cap   = init;
        max_n = 8;   /* indices 0..8 are known */

        /* Initial values of the sequence. */
        cache[0] =  1;  /* a(0) = 1 */
        cache[1] =  1;  /* a(1) = 1 */
        cache[2] =  1;  /* a(2) = 1 */
        cache[3] =  0;  /* a(3) = 0 */
        cache[4] =  0;  /* a(4) = 0 */
        cache[5] = -1;  /* a(5) = -1 */
        cache[6] = -1;  /* a(6) = -1 */
        cache[7] = -1;  /* a(7) = -1 */
        cache[8] =  0;  /* a(8) = 0 */
    }

    /* Ensure the internal array is large enough for index n. */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap << 1 : 16;
        while ((size_t)n + 1 > newcap) newcap <<= 1;
        cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* Compute new terms if required. */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* a(i) = a(i-1) - a(i-3) + a(i-4) - a(i-5) + a(i-7) - a(i-8) */
            seq_t val = cache[i-1];          /* a(i-1) */
            val -= cache[i-3];               /* - a(i-3) */
            val += cache[i-4];               /* + a(i-4) */
            val -= cache[i-5];               /* - a(i-5) */
            val += cache[i-7];               /* + a(i-7) */
            val -= cache[i-8];               /* - a(i-8) */
            cache[i] = val;
        }
        max_n = n;
    }

    return cache[n];
}
