/*
 * A063317  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          44 ).
 * https://oeis.org/A063317
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13 Initial values
 *            (offset = 2): a(2) = -1, a(3) = 25, a(4) = 60, a(5) = 95, a(6) =
 *            128, a(7) = 163, a(8) = 196, a(9) = 235, a(10) = 264, a(11) =
 *            303, a(12) = 332, a(13) = 373
 * Range:     i8 n <= 5, i16 n <= 46, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_len = 0;   /* first unused index, starts at 0 */

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is undefined; return 0 safely. */
    if (n < 2) return (seq_t)0;

    /* Grow cache to accommodate n. */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;

        /* Compute values from old cached_len up to n. */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 2) {
                cache[i] = (seq_t)0;
            } else if (i <= 13) {
                /* Initial values for 2..13 */
                switch (i) {
                    case 2: cache[i] = (seq_t)(-1); break;
                    case 3: cache[i] = (seq_t)(25); break;
                    case 4: cache[i] = (seq_t)(60); break;
                    case 5: cache[i] = (seq_t)(95); break;
                    case 6: cache[i] = (seq_t)(128); break;
                    case 7: cache[i] = (seq_t)(163); break;
                    case 8: cache[i] = (seq_t)(196); break;
                    case 9: cache[i] = (seq_t)(235); break;
                    case 10: cache[i] = (seq_t)(264); break;
                    case 11: cache[i] = (seq_t)(303); break;
                    case 12: cache[i] = (seq_t)(332); break;
                    case 13: cache[i] = (seq_t)(373); break;
                }
            } else {
                /* Recurrence: a(n) = a(n-4)+a(n-6)-a(n-10) */
                cache[i] = cache[i-4] + cache[i-6] - cache[i-10];
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
