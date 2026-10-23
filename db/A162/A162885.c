/*
 * A162885  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A162885 – Number of reduced words of length n in the
   Coxeter group on 45 generators with relations (S_i)^2 = (S_i S_j)^3 = I.
   Recurrence:  a(0)=1, a(1)=45, a(2)=1980,
                 a(n)=43*a(n-1)+43*a(n-2)-946*a(n-3)   (n≥4)
                 a(3)=43*a(2)+43*a(1)-946*a(0)+1   (constant 1 from the denominator of the G.f.) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of valid entries already computed */
    static idx_t cache_cap = 0;   /* allocated size of cache array */

    /* Ensure we have enough room to store up to index n */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure – unlikely in tests */
        cache        = tmp;
        cache_cap    = (idx_t)new_cap;
    }

    /* Compute all missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)45;
        } else if (i == 2) {
            cache[i] = (seq_t)1980;
        } else if (i == 3) {
            cache[i] = (seq_t)43 * cache[i-1] +
                       (seq_t)43 * cache[i-2] -
                       (seq_t)946 * cache[i-3] + (seq_t)1;
        } else {   /* i >= 4 */
            cache[i] = (seq_t)43 * cache[i-1] +
                       (seq_t)43 * cache[i-2] -
                       (seq_t)946 * cache[i-3];
        }
    }

    cache_len = n + 1;
    return cache[n];
}
