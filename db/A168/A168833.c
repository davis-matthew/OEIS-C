/*
 * A168833  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, for 1 ≤ n ≤ 19: a(n) = 10 * (a(0)+a(1)+…+a(n‑1)) + 2,
 *            a(20) = 10 * (a(1)+…+a(19)) – 55 * a(0) + 1, for n ≥ 21: a(n) =
 *            10 * (a(n‑1)+…+a(n‑19)) – 55 * a(n‑20)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A168833 – Number of reduced words of length n in the Coxeter group on
    12 generators with all relations (S_i)^2 = (S_i S_j)^20 = I.  */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;      /* memoised values */
static idx_t  cache_size = 0;    /* number of stored terms */

seq_t a(idx_t n)
{
    /* Ensure cache holds at least terms 0 … n */
    if (n >= cache_size) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        /* initialise cache[0] if it was not yet set */
        if (cache_size == 0) cache[0] = 1;
        /* generate missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) continue;          /* already set to 1 */
            if (i <= 19) {                 /* 1 ≤ i ≤ 19 */
                seq_t sum = 0;
                for (idx_t k = 0; k < i; ++k) sum += cache[k];
                cache[i] = 10 * sum + 2;
            } else if (i == 20) {           /* i = 20 */
                seq_t sum19 = 0;
                for (idx_t k = 1; k <= 19; ++k) sum19 += cache[k];
                cache[i] = 10 * sum19 - 55 * cache[0] + 1;
            } else {                        /* i ≥ 21 */
                seq_t sum19 = 0;
                for (idx_t k = 1; k <= 19; ++k) sum19 += cache[i - k];
                cache[i] = 10 * sum19 - 55 * cache[i - 20];
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
