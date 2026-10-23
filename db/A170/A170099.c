/*
 * A170099  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for 1≤n≤36: a(n)=2+40*∑_{k=0}^{n-1}a(k); for n=37:
 *            a(37)=1+40*∑_{k=1}^{36}a(k)−820*a(0); for n≥38:
 *            a(n)=40*∑_{k=n-36}^{n-1}a(k)−820*a(n-37); (derived from the
 *            rational generating function. This recurrence computes each term
 *            in O(1) amortised time.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* allocated capacity   */
    static size_t cache_size = 0;   /* number of values computed */

    /* extend cache if needed */
    if (n + 1 > cache_size) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 128 : cache_cap;
            while (new_cap < needed) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i <= 36) {
                seq_t sum = (seq_t)0;
                for (size_t j = 0; j < i; ++j) sum += cache[j];
                cache[i] = (seq_t)2 + sum * (seq_t)40;
            } else if (i == 37) {
                seq_t sum = (seq_t)0;
                for (size_t j = 1; j <= 36; ++j) sum += cache[j];
                cache[37] = (seq_t)1 + sum * (seq_t)40 -
                            cache[0] * (seq_t)820;
            } else { /* i ≥ 38 */
                seq_t sum = (seq_t)0;
                for (size_t j = i - 36; j < i; ++j) sum += cache[j];
                cache[i] = sum * (seq_t)40 -
                           cache[i - 37] * (seq_t)820;
            }
        }
        cache_size = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
