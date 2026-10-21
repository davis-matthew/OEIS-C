/*
 * A063340  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          67 ).
 * https://oeis.org/A063340
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 41, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A063340
 *  Recurrence:  a(n) = a(n-4) + a(n-6) - a(n-10)    for n > 13
 *  Offset:     2   (a(2) is the first listed term)
 *  Initial terms: a(2) = -1, a(3) = 155, a(4) = 341, a(5) = 528,
 *                 a(6) = 715, a(7) = 902, a(8) = 1089, a(9) = 1276,
 *                 a(10) = 1463, a(11) = 1650, a(12) = 1837, a(13) = 2022
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* calls with n < 2 are outside the defined range – return 0 */
    if (n < 2) return (seq_t)0;

    /* static state for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;   /* allocated size (max index + 1)   */
    static idx_t  max_done   = 1;   /* highest n for which a[n] is known */

    /* initialise the cache with known values up to n = 13 */
    if (cache == NULL) {
        /* indices 0..13 are needed (14 elements) */
        cache_size = 14;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed */

        /* unused indices 0 and 1 */
        cache[0] = cache[1] = (seq_t)0;

        /* initial terms */
        cache[2]  = (seq_t)-1;
        cache[3]  = (seq_t)155;
        cache[4]  = (seq_t)341;
        cache[5]  = (seq_t)528;
        cache[6]  = (seq_t)715;
        cache[7]  = (seq_t)902;
        cache[8]  = (seq_t)1089;
        cache[9]  = (seq_t)1276;
        cache[10] = (seq_t)1463;
        cache[11] = (seq_t)1650;
        cache[12] = (seq_t)1837;
        cache[13] = (seq_t)2022;

        max_done = 13;
    }

    /* grow the cache if required */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failed */
        cache      = tmp;
        /* new elements are uninitialised; they will be filled below */
        cache_size = new_size;
    }

    /* compute missing terms using the recurrence */
    if (n > max_done) {
        for (idx_t i = max_done + 1; i <= n; ++i) {
            /* recurrence is safe for i > 13 because the referenced indices are defined */
            cache[i] = cache[i-4] + cache[i-6] - cache[i-10];
        }
        max_done = n;
    }

    return cache[n];
}
