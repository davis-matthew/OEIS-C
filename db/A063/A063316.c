/*
 * A063316  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          43 ).
 * https://oeis.org/A063316
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 43, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A063316  Dimension of the space of weight n cuspidal newforms for Gamma1(43)
 *  Formula:  a(n) = a(n-4) + a(n-6) - a(n-10)   for n > 13
 *  initial values:
 *    a(2) = -1, a(3) = 57, a(4) = 133, a(5) = 210,
 *    a(6) = 287, a(7) = 364, a(8) = 441, a(9) = 518,
 *    a(10) = 595, a(11) = 672, a(12) = 749, a(13) = 824
 *  Offset: 2   (a(2) is the first term)
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2)               /* values below the offset are irrelevant */
        return 0;

    static seq_t *cache = NULL;      /* array of already computed terms */
    static idx_t  cache_size = 0;   /* allocated size of the array        */
    static idx_t  computed   = 13;   /* largest index that has been filled */
    static bool   initialized = false;

    /* First call: allocate the array and store the initial values. */
    if (!initialized) {
        cache_size = 14;                       /* indices 0 .. 13 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = 0;                      /* harmless defaults */

        /* Initial values for 2 .. 13 (indices 0 and 1 stay 0). */
        cache[2]  = -1;
        cache[3]  = 57;
        cache[4]  = 133;
        cache[5]  = 210;
        cache[6]  = 287;
        cache[7]  = 364;
        cache[8]  = 441;
        cache[9]  = 518;
        cache[10] = 595;
        cache[11] = 672;
        cache[12] = 749;
        cache[13] = 824;

        initialized = true;
    }

    /* Expand the array if the requested index is beyond the current size. */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) {          /* allocation failed – return a safe value */
            return 0;
        }
        /* Initialise the newly added positions to 0. */
        for (idx_t i = cache_size; i < new_size; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute any missing values up to the requested index. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 13) {
            /* Already initialised – nothing to do. */
            continue;
        }
        /* Recurrence: a(i) = a(i-4) + a(i-6) - a(i-10) */
        cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
    }

    computed = (n > computed) ? n : computed;  /* update the largest computed */
    return cache[n];
}
