/*
 * A129206  First sequence in solution to congruent number 5 problem.
 * https://oeis.org/A129206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 4, i64 n <= 6, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 1, a(2) = -12, a(3) = -2257, a(4) = 1494696.
   For n ≥ 5: a(n) = (144*a(n-1)*a(n-3) + 2257*a(n-2)*a(n-2)) / a(n-4)   (exact division). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache array and its current length */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* first call: initialise cache with the first five terms */
    if (cache == NULL) {
        cache_len = 5;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) {
            return (seq_t)0; /* allocation failed */
        }
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)(-12);
        cache[3] = (seq_t)(-2257);
        cache[4] = (seq_t)1494696;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* enlarge cache to accommodate up to index n */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        return (seq_t)0; /* allocation failed */
    }
    cache = tmp;
    idx_t old_len = cache_len;
    cache_len = n + 1;

    /* compute all terms that were not previously cached */
    for (idx_t i = old_len; i <= n; ++i) {
        seq_t num1 = 144 * cache[i - 1] * cache[i - 3];
        seq_t num2 = 2257 * cache[i - 2] * cache[i - 2];
        cache[i] = (num1 + num2) / cache[i - 4]; /* division is exact */
    }

    return cache[n];
}
