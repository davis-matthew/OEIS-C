/*
 * A162850  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence a(0)=1, a(1)=36, a(2)=1260,
 *            a(3)=34*a(2)+34*a(1)-595*a(0)+1,
 *            a(n)=34*a(n-1)+34*a(n-2)-595*a(n-3) for n≥4 (generating function
 *            (t³+2t²+2t+1)/(1-34t-34t²+595t³))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_last = (size_t)(-1); /* no value computed yet */

    /* grow cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap == 0 ? 1 : cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;          /* allocation failure – unlikely in tests */
        if (cache) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (size_t k = cache_last + 1; k <= (size_t)n; ++k) {
        if (k == 0) {
            cache[0] = (seq_t)1;
        } else if (k == 1) {
            cache[1] = (seq_t)36;
        } else if (k == 2) {
            cache[2] = (seq_t)1260;
        } else if (k == 3) {
            cache[3] = (cache[2] * (seq_t)34) + (cache[1] * (seq_t)34)
                       - (cache[0] * (seq_t)595) + (seq_t)1;
        } else {
            cache[k] = (cache[k-1] * (seq_t)34) + (cache[k-2] * (seq_t)34)
                       - (cache[k-3] * (seq_t)595);
        }
    }

    cache_last = (size_t)n;
    return cache[n];
}
