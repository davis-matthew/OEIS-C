/*
 * A168875  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(0) = 1, a(1) = 6, a(n) = 5*a(n-1) for 2 ≤ n ≤ 20, a(21)
 *            = 4*∑_{i=1}^{20}a(21-i) - 10*a(0) + 1, a(n) =
 *            4*∑_{i=1}^{20}a(n-i) - 10*a(n-21) for n ≥ 22
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_last = (idx_t)(-1);   /* no values computed yet */

    /* Allocate minimal initial cache and set a(0)=1 */
    if (cache_size == 0) {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache_last = 0;
    }

    /* Extend cache if needed */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute new values up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;          /* a(0) */
        } else if (i == 1) {
            val = (seq_t)6;          /* a(1) */
        } else if (i <= 20) {
            /* a(i) = 5 * a(i-1) for 2 ≤ i ≤ 20 */
            val = (seq_t)5 * cache[i - 1];
        } else if (i == 21) {
            /* a(21) = 4*sum_{k=1}^{20}a(21-k) -10*a(0) + 1 */
            seq_t sum = 0;
            for (idx_t j = 1; j <= 20; ++j) sum += cache[j];
            val = (seq_t)4 * sum - (seq_t)10 * cache[0] + (seq_t)1;
        } else {
            /* a(n) = 4*sum_{k=1}^{20}a(n-k) -10*a(n-21) for n ≥ 22 */
            seq_t sum = 0;
            for (idx_t j = i - 20; j < i; ++j) sum += cache[j];
            val = (seq_t)4 * sum - (seq_t)10 * cache[i - 21];
        }
        cache[i] = val;
    }

    cache_last = n;
    return cache[n];
}
