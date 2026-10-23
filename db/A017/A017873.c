/*
 * A017873  Expansion of 1/(1-x^8-x^9-x^10-x^11-x^12-x^13-x^14-x^15).
 * https://oeis.org/A017873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            a(n-8)+a(n-9)+a(n-10)+a(n-11)+a(n-12)+a(n-13)+a(n-14)+a(n-15)
 *            for n>14; with a(0)=1, a(1..7)=0, a(8..15)=1
 * Range:     u8 n <= 41, u16 n <= 71, u32 n <= 129, u64 n <= 247,
 *            u128 n <= 482, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of entries currently stored */

    /* initialisation on first call */
    if (cache == NULL) {
        cache_size = 16;                  /* indices 0 .. 15 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) abort();              /* allocation failure */

        cache[0] = (seq_t)1;
        for (idx_t i = 1; i < 8; ++i)
            cache[i] = (seq_t)0;
        for (idx_t i = 8; i < 16; ++i)
            cache[i] = (seq_t)1;
    }

    /* grow cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;           /* need slots 0 .. n inclusive */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!new_cache) abort();          /* allocation failure */
        cache = new_cache;

        /* compute new entries iteratively */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (int d = 8; d <= 15; ++d) {
                if (i >= (idx_t)d)
                    sum += cache[i - d];
            }
            cache[i] = sum;
        }
        cache_size = new_size;
    }

    return cache[n];
}
