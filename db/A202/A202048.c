/*
 * A202048  Number of (n+2) X 6 binary arrays avoiding patterns 001 and 110 in
 *          rows and columns.
 * https://oeis.org/A202048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 66, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence formula: 
   a(1) = 636,    a(2) = 1968,  a(3) = 4980,  a(4) = 11016,
   a(5) = 22092,  a(6) = 41088, a(7) = 71964, a(8) = 120000.
   For n > 8:  a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4)
               + 56*a(n-5) - 28*a(n-6) + 8*a(n-7) - a(n-8). */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static idx_t  last_computed = 0;

    /* initialise cache with the first 8 terms if needed */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            /* memory allocation failed – abort */
            abort();
        }
        cache[0] = (seq_t)636;     /* a(1) */
        cache[1] = (seq_t)1968;    /* a(2) */
        cache[2] = (seq_t)4980;    /* a(3) */
        cache[3] = (seq_t)11016;   /* a(4) */
        cache[4] = (seq_t)22092;   /* a(5) */
        cache[5] = (seq_t)41088;   /* a(6) */
        cache[6] = (seq_t)71964;   /* a(7) */
        cache[7] = (seq_t)120000;  /* a(8) */
        last_computed = 8;
    }

    /* if requested n is already computed, simply return it */
    if (n <= last_computed) {
        return cache[n - 1];
    }

    /* grow cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n > new_cap) {
            new_cap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val  = (seq_t)8   * cache[(i - 2)];   /* 8*a(i-1)   */
        val -= (seq_t)28  * cache[(i - 3)];          /* -28*a(i-2) */
        val += (seq_t)56  * cache[(i - 4)];          /* +56*a(i-3) */
        val -= (seq_t)70  * cache[(i - 5)];          /* -70*a(i-4) */
        val += (seq_t)56  * cache[(i - 6)];          /* +56*a(i-5) */
        val -= (seq_t)28  * cache[(i - 7)];          /* -28*a(i-6) */
        val += (seq_t)8   * cache[(i - 8)];          /* +8*a(i-7)  */
        val -= cache[(i - 9)];                       /* -a(i-8)    */
        cache[i - 1] = val;
    }
    last_computed = n;
    return cache[n - 1];
}
