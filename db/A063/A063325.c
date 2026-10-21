/*
 * A063325  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          52 ).
 * https://oeis.org/A063325
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 45, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A063325
   Formula (for n > 13):  a(n) = a(n-4) + a(n-6) - a(n-10)
   Initial values:
   a(2) = -1
   a(3) = 37
   a(4) = 86
   a(5) = 135
   a(6) = 182
   a(7) = 231
   a(8) = 278
   a(9) = 331
   a(10) = 374
   a(11) = 427
   a(12) = 470
   a(13) = 525
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2)   /* behavior for n < 2 not used by OEIS */
        return (seq_t)0;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* allocated size */
    static idx_t max_computed = 0;  /* biggest n already computed */

    /* ensure cache big enough */
    idx_t required = n + 1;
    if (required < 14)          /* we need space for the initial values up to 13 */
        required = 14;

    if (required > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!tmp) {             /* allocation failed – return 0 (unlikely in tests) */
            return (seq_t)0;
        }
        cache = tmp;
        /* initialise the newly allocated part to 0 */
        for (idx_t i = cache_size; i < required; ++i)
            cache[i] = (seq_t)0;
        cache_size = required;
    }

    /* compute new values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 13) {
            /* initial values (array index = n) */
            static const seq_t init[14] = {
                0, 0, -1, 37, 86, 135, 182, 231,
                278, 331, 374, 427, 470, 525
            };
            cache[i] = init[i];
        } else {
            /* recurrence for n > 13 */
            cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
        }
    }
    max_computed = n;
    return cache[n];
}
