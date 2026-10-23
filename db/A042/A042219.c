/*
 * A042219  Denominators of continued fraction convergents to sqrt(635).
 * https://oeis.org/A042219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1, a(1)=5, a(2)=251, a(3)=1260; a(n)=252*a(n-2)-a(n-4) for
 *            n>3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t   cache_cap = 0;

    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        seq_t *new_cache = (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;

        for (size_t i = cache_cap; i < need; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)5;
            } else if (i == 2) {
                cache[2] = (seq_t)251;
            } else if (i == 3) {
                cache[3] = (seq_t)1260;
            } else {
                cache[i] = ((seq_t)252 * cache[i-2]) - cache[i-4];
            }
        }
        cache_cap = need;
    }
    return cache[(size_t)n];
}
