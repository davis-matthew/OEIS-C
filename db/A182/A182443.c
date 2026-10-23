/*
 * A182443  a(0)=0, a(1)=1, for n>1, a(n) = floor(a(n-1)/n) + a(n-2)*2.
 * https://oeis.org/A182443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(n)=floor(a(n-1)/n)+a(n-2)*2 for n>1
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 64, u64 n <= 128,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return 0;          /* Allocation failure is unlikely */
        cache[0] = 0;
        cache[1] = 1;
        cache_len = 2;
    }

    if ((idx_t)n >= cache_len) {
        if ((size_t)n >= cache_cap) {
            size_t new_cap = cache_cap > 0 ? cache_cap : 16;
            while (new_cap <= (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;
            for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
            cache = tmp;
            cache_cap = new_cap;
        }

        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = cache[i-1] / ((seq_t)i) + cache[i-2] * ((seq_t)2);
        }
        cache_len = n + 1;
    }
    return cache[n];
}
