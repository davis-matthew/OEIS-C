/*
 * A172397  a(n) = a(n-1) + a(n-2) - a(n-3) - a(n-8), starting
 *          1,1,2,2,3,3,4,4.
 * https://oeis.org/A172397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) - a(n-8) Initial terms: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=2, a(4)=3, a(5)=3, a(6)=4, a(7)=4
 * Range:     i8 n <= 25, i16 n <= 44, i32 n <= 93, i64 n <= 187,
 *            i128 n <= 377, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* dynamic memoization */
    static seq_t *cache = NULL;
    static idx_t size = 0;          /* allocated array length */
    static idx_t computed = 0;      /* highest index computed + 1 */

    /* initialise first 8 terms on first call */
    if (cache == NULL) {
        size = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * size);
        if (!cache) return 0;          /* allocation failed */
        const seq_t init[8] = {1, 1, 2, 2, 3, 3, 4, 4};
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init[i];
        computed = 8;
    }

    /* ensure the cache is large enough */
    if (n + 1 > size) {
        idx_t new_size = size;
        while (new_size <= n) {
            new_size <<= 1;          /* grow by factor 2 */
            if (new_size == 0) {      /* overflow guard */
                new_size = n + 1;
                break;
            }
        }
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        size = new_size;
    }

    /* compute missing values */
    if (n >= computed) {
        for (idx_t i = computed; i <= n; ++i) {
            if (i < 8) {
                /* already initialised */
                continue;
            }
            cache[i] = cache[i - 1] + cache[i - 2]
                     - cache[i - 3] - cache[i - 8];
        }
        computed = n + 1;
    }

    return cache[n];
}
