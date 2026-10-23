/*
 * A017896  Expansion of
 *          1/(1-x^10-x^11-x^12-x^13-x^14-x^15-x^16-x^17-x^18-x^19-x^20).
 * https://oeis.org/A017896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-10)+a(n-11)+…+a(n-20) (n>19) with a(0)=1, a(k)=0 for
 *            1≤k≤9, a(k)=1 for 10≤k≤19
 * Range:     u8 n <= 48, u16 n <= 80, u32 n <= 146, u64 n <= 277,
 *            u128 n <= 539, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store already computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* enlarge cache if needed */
    if ((idx_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);      /* out of memory */
        cache = new_cache;

        /* initialise new entries */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                cache[i] = 1;
            } else if (i <= 9) {
                cache[i] = 0;
            } else {
                seq_t sum = 0;
                for (int k = 10; k <= 20; ++k) {
                    if (i >= (idx_t)k) {
                        sum += cache[i - k];
                    }
                }
                cache[i] = sum;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
