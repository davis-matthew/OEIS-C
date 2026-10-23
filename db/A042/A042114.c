/*
 * A042114  Numerators of continued fraction convergents to sqrt(582).
 * https://oeis.org/A042114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 386*a(n-2) - a(n-4) (n ≥ 4)
   Initial values: a(0)=24, a(1)=193, a(2)=9288, a(3)=74497 */
#include <stdlib.h>
#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Initial values for n = 0..3 */
    static const seq_t init[] = {24, 193, 9288, 74497};

    /* Ensure the cache can hold indices up to n */
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = NULL;

        if (cache == NULL) {
            tmp = (seq_t*)malloc(new_size * sizeof(seq_t));
        } else {
            tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        }
        if (tmp == NULL) {
            abort();  /* allocation failed */
        }
        cache = tmp;

        /* Compute missing values */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i < 4) {
                cache[i] = init[i];
            } else {
                cache[i] = (seq_t)(386) * cache[i - 2] - cache[i - 4];
            }
        }

        cache_size = new_size;
    }

    return cache[(size_t)n];
}
