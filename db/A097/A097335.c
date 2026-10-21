/*
 * A097335  a(n) = Sum_{k=0..n} C(n-k, floor(k/2))*3^k.
 * https://oeis.org/A097335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 9*a(n-3) (for n ≥ 3), a(0)=1, a(1)=4, a(2)=4
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with first three terms if necessary */
    if (cache_size == 0) {
        cache_size = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return 0;            /* allocation failure – undefined behaviour */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)4;
    } else if (cache_size <= n) {        /* extend cache up to n */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) return 0;              /* allocation failure – undefined behaviour */
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)4;
            } else if (i == 2) {
                cache[i] = (seq_t)4;
            } else {
                cache[i] = cache[i - 1] + (seq_t)9 * cache[i - 3];
            }
        }
        cache_size = n + 1;
    }

    return cache[n];
}
