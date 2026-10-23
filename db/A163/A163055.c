/*
 * A163055  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1)+10*a(n-2)+10*a(n-3)-55*a(n-4) (n≥5) with
 *            initial values a(0)=1, a(1)=12, a(2)=132, a(3)=1452, a(4)=15906
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with the first 5 terms */
    if (cache_size == 0) {
        cache_size = 5;
        cache = (seq_t *)calloc((size_t)cache_size, sizeof(seq_t));
        if (!cache) return 0;            /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)12;
        cache[2] = (seq_t)132;
        cache[3] = (seq_t)1452;
        cache[4] = (seq_t)15906;
    }

    if (n < cache_size)
        return cache[n];

    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;            /* allocation failed */
    cache = tmp;

    for (idx_t i = cache_size; i <= n; ++i) {
        /* recurrence only for i ≥ 5;  cache[0..4] are already set */
        seq_t t = cache[i-1] * (seq_t)10;
        t += cache[i-2] * (seq_t)10;
        t += cache[i-3] * (seq_t)10;
        cache[i] = t - cache[i-4] * (seq_t)55;
    }

    cache_size = new_size;
    return cache[n];
}
