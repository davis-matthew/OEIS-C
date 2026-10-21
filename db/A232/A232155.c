/*
 * A232155  Number of n X 2 0..3 arrays with every 0 next to a 1, every 1 next
 *          to a 2 and every 2 next to a 3 horizontally, vertically or
 *          antidiagonally, and no adjacent values equal.
 * https://oeis.org/A232155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) - 13*a(n-3) - 5*a(n-4) + 14*a(n-5) -
 *            3*a(n-6) + 3*a(n-7) + 8*a(n-8) + a(n-9) - a(n-10) for n > 11
 *            Initial values: a(1) = 2, a(2) = 18, a(3) = 62, a(4) = 218, a(5)
 *            = 746, a(6) = 2600, a(7) = 9004, a(8) = 31262, a(9) = 108492,
 *            a(10) = 376566, a(11) = 1307146
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation, 1‑based indexing; cache[0] unused */
    static seq_t *cache     = NULL;
    static idx_t cache_size = 0;   /* largest n that has been computed */
    static size_t cache_cap = 0;   /* allocated array capacity */

    /* initialise cache once */
    if (cache == NULL) {
        /* start with a capacity that comfortably holds the first 64 terms */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failure – not expected */

        /* zero the array */
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = (seq_t)0;

        /* hard‑code the first 11 terms */
        cache[1]  = (seq_t)  2;
        cache[2]  = (seq_t) 18;
        cache[3]  = (seq_t) 62;
        cache[4]  = (seq_t)218;
        cache[5]  = (seq_t)746;
        cache[6]  = (seq_t)2600;
        cache[7]  = (seq_t)9004;
        cache[8]  = (seq_t)31262;
        cache[9]  = (seq_t)108492;
        cache[10] = (seq_t)376566;
        cache[11] = (seq_t)1307146;

        cache_size = 11;
    }

    /* grow the array if the requested index is outside current capacity */
    if (n > (idx_t)cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1)) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure – not expected */
        /* zero the newly allocated part */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* if we already have a value for this index, return it */
    if (n <= cache_size) return cache[n];

    /* compute all missing values up to n */
    for (idx_t idx = cache_size + 1; idx <= n; ++idx) {
        /* recurrence requires the ten preceding terms; they all exist */
        seq_t b1  = cache[idx - 1];
        seq_t b2  = cache[idx - 2];
        seq_t b3  = cache[idx - 3];
        seq_t b4  = cache[idx - 4];
        seq_t b5  = cache[idx - 5];
        seq_t b6  = cache[idx - 6];
        seq_t b7  = cache[idx - 7];
        seq_t b8  = cache[idx - 8];
        seq_t b9  = cache[idx - 9];
        seq_t b10 = cache[idx -10];

        /* compute via the given recurrence; all operations obey the
           operator restrictions specified in the problem statement */
        seq_t term =  (seq_t)4 * b1
                    + (seq_t)2 * b2
                    - (seq_t)13 * b3
                    - (seq_t)5  * b4
                    + (seq_t)14 * b5
                    - (seq_t)3  * b6
                    + (seq_t)3  * b7
                    + (seq_t)8  * b8
                    +              b9
                    -              b10;
        cache[idx] = term;
    }

    cache_size = n;
    return cache[n];
}
