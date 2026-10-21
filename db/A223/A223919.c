/*
 * A223919  Number of 2 X n 0..2 arrays with rows and antidiagonals unimodal
 *          and columns nondecreasing.
 * https://oeis.org/A223919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) + *
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) (n>9) *
 *            Initial values: a(1)=6, a(2)=36, a(3)=158, a(4)=548, a(5)=1600,
 *            * a(6)=4102, a(7)=9503, a(8)=20299, a(9)=40570
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 47, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* cache of already computed terms; cache[0] == a(1) */
    static seq_t *cache = NULL;
    static idx_t cache_limit = 0;          /* number of entries cached */

    /* first nine terms of the sequence */
    static const seq_t init[9] = {
        (seq_t)6, (seq_t)36, (seq_t)158, (seq_t)548,
        (seq_t)1600, (seq_t)4102, (seq_t)9503,
        (seq_t)20299, (seq_t)40570
    };

    /* extend cache if necessary */
    if (cache_limit < n) {
        idx_t new_limit = n;
        seq_t *new_cache;

#ifdef __cplusplus
        new_cache = new seq_t[new_limit];
#else
        new_cache = (seq_t *)malloc(new_limit * sizeof(seq_t));
#endif
        if (!new_cache) return 0;           /* allocation failed */

        /* copy old data */
        if (cache) {
            for (idx_t i = 0; i < cache_limit; ++i) {
                new_cache[i] = cache[i];
            }
        }

        /* fill in new entries */
        for (idx_t i = cache_limit; i < new_limit; ++i) {
            if (i < 9) {
                new_cache[i] = init[i];
            } else {
                seq_t val =
                    (seq_t)9   * new_cache[i - 1]
                  - (seq_t)36  * new_cache[i - 2]
                  + (seq_t)84  * new_cache[i - 3]
                  - (seq_t)126 * new_cache[i - 4]
                  + (seq_t)126 * new_cache[i - 5]
                  - (seq_t)84  * new_cache[i - 6]
                  + (seq_t)36  * new_cache[i - 7]
                  - (seq_t)9   * new_cache[i - 8]
                  +           new_cache[i - 9];
                new_cache[i] = val;
            }
        }

        /* free old cache */
        if (cache) {
#ifdef __cplusplus
            delete [] cache;
#else
            free(cache);
#endif
        }

        cache       = new_cache;
        cache_limit = new_limit;
    }

    return cache[n - 1];
}
