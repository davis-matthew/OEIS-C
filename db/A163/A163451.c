/*
 * A163451  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n‑1)+15*a(n‑2)+15*a(n‑3)+15*a(n‑4)-120*a(n‑5) for n
 *            >= 6; a(5) = 15*a(4)+15*a(3)+15*a(2)+15*a(1)-120*a(0)+1. Initial
 *            values: a(0)=1,a(1)=17,a(2)=272,a(3)=4352,a(4)=69632
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>    /* malloc, realloc, free */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms known (next free index) */
    static size_t cache_cap = 0;   /* capacity of cache array            */

    /* initialise cache with first five terms */
    if (cache_len == 0) {
        /* ensure at least room for index 5 (a(5)) */
        cache_cap = 6;
        if ((idx_t)cache_cap < n + 1)
            cache_cap = (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failed            */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)17;
        cache[2] = (seq_t)272;
        cache[3] = (seq_t)4352;
        cache[4] = (seq_t)69632;
        cache_len = 5;                       /* next free index              */
    }

    /* grow and fill cache to include requested n */
    if ((idx_t)cache_len <= n) {
        idx_t needed = n + 1;
        if ((size_t)needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap : 1;
            while (new_cap < (size_t)needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;              /* allocation failed            */
            cache = tmp;
            cache_cap = new_cap;
        }

        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t term;
            if (i == 5) {
                term = 15 * cache[4] + 15 * cache[3] + 15 * cache[2]
                      + 15 * cache[1] - 120 * cache[0] + 1;
            } else {
                term = 15 * cache[i-1] + 15 * cache[i-2] + 15 * cache[i-3]
                      + 15 * cache[i-4] - 120 * cache[i-5];
            }
            cache[i] = term;
        }
        cache_len = (size_t)(n + 1);
    }

    return cache[n];
}
