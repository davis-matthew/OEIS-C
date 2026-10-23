/*
 * A118444  a(n) = A118443(n)/(n+1), where A118443 is the row sums of triangle
 *          A118441.
 * https://oeis.org/A118444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) + 12*a(n-2) +
 *            86*a(n-4) + 300*a(n-6) + 625*a(n-8) = 0 for n ≥ 8 Initial values
 *            (n = 0 … 7): 1, -1, 1, -9, -31, 79, 161, -249 These values are
 *            given directly from the sequence
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 26, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* computed values up to current size-1   */
    static size_t cache_len = 0;     /* number of slots allocated             */
    static const seq_t init[8] = { (seq_t)1, (seq_t)-1, (seq_t)1, (seq_t)-9,
                                   (seq_t)-31, (seq_t)79, (seq_t)161, (seq_t)-249 };

    size_t idx = (size_t)n;
    if (idx < 8)                     /* directly return the known values     */
        return init[idx];

    if (cache_len <= idx) {           /* need to compute more terms            */
        size_t new_len = idx + 1;     /* array size needed                    */
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();      /* allocation failed                    */

        /* initialise first 8 terms if we hadn't yet */
        if (cache_len < 8) {
            size_t j;
            for (j = cache_len; j < 8; ++j)
                new_cache[j] = init[j];
        }

        /* compute terms from the next missing index up to idx */
        size_t i;
        for (i = (cache_len < 8 ? 8 : cache_len); i <= idx; ++i) {
            new_cache[i] = (seq_t)-12 * new_cache[i-2]
                           + (seq_t)-86 * new_cache[i-4]
                           + (seq_t)-300 * new_cache[i-6]
                           + (seq_t)-625 * new_cache[i-8];
        }

        cache     = new_cache;
        cache_len = new_len;
    }

    return cache[idx];
}
