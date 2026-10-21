/*
 * A073748  a(n) = S(n)*S(n-1), where S(n) are the generalized tribonacci
 *          numbers A001644.
 * https://oeis.org/A073748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = S(n)*S(n-1) (generalized tribonacci), recurrence: a(n) =
 *            2*a(n-1) + 3*a(n-2) + 6*a(n-3) - a(n-4) - a(n-6) initial values:
 *            a(0) = -3 a(1) = 3 a(2) = 3 a(3) = 21 a(4) = 77 a(5) = 231 a(6)
 *            = 819
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 18, i64 n <= 36, i128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_max = 0;          /* highest index computed */
    static seq_t init[7] = {(seq_t)-3, (seq_t)3, (seq_t)3,
                            (seq_t)21, (seq_t)77, (seq_t)231, (seq_t)819};

    if (n <= 6) {
        return init[n];
    }

    if (cache == NULL) {
        /* allocate cache for indices 0..6 */
        cache_size = 7;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* out of memory, unlikely */
        for (idx_t i = 0; i < 7; ++i) {
            cache[i] = init[i];
        }
        cache_max = 6;
    }

    if (n >= cache_size) {
        /* grow cache to accommodate index n */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* out of memory */
        cache = tmp;
        cache_size = new_size;
    }

    /* extend computation up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = (seq_t)2 * cache[i - 1]
                 + (seq_t)3 * cache[i - 2]
                 + (seq_t)6 * cache[i - 3]
                 - cache[i - 4]
                 - cache[i - 6];
    }
    cache_max = n;

    return cache[n];
}
