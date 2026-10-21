/*
 * A063304  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          31 ).
 * https://oeis.org/A063304
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 46, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A063304
   Formula: a(n) = a(n-4) + a(n-6) - a(n-10)   for n > 13
   Initial values: a(2) = -1, a(3) = 26, a(4) = 65, a(5) = 105,
                    a(6) = 145, a(7) = 185, a(8) = 225, a(9) = 265,
                    a(10) = 305, a(11) = 345, a(12) = 385, a(13) = 423
   Offset: 2
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2.  For n < 2 we simply
       return 0 (the value is never used in correct OEIS indexing).   */
    if (n < 2) return 0;

    /* --- memoisation array ---------------------------------------- */
    static seq_t *cache = NULL;   /* pointer to computed terms      */
    static size_t cache_size = 0; /* number of elements currently stored */

    size_t idx = (size_t)n;                /* index in the cache array */
    if (idx < cache_size)                 /* already computed ?        */
        return cache[idx];

    /* --- grow the array if necessary -------------------------------- */
    size_t new_size = idx + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;                   /* allocation failed          */
    cache = tmp;

    /* --- initial terms up to n = 13 --------------------------------- */
    static const seq_t base[14] = {
        [0] = 0, [1] = 0,
        [2] = -1, [3] = 26,  [4] = 65,  [5] = 105,
        [6] = 145, [7] = 185, [8] = 225, [9] = 265,
       [10] = 305, [11] = 345, [12] = 385, [13] = 423
    };

    /* --- compute all new entries ---------------------------------- */
    for (size_t i = cache_size; i < new_size; ++i) {
        if (i < sizeof(base)/sizeof(base[0]))                      /* base part */
            cache[i] = base[i];
        else                                                             /* recurrence */
            cache[i] = cache[i-4] + cache[i-6] - cache[i-10];
    }
    cache_size = new_size;
    return cache[idx];
}
