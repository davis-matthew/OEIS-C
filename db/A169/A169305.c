/*
 * A169305  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^30 = I.
 * https://oeis.org/A169305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(1) = 4; a(n) = 3 * a(n-1) for 2 ≤ n < 30; a(n) = -3
 *            * a(n-30) + 2 * Σ_{k=1..29} a(n-k) for n ≥ 30
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache */
    static seq_t *cache = 0;          /* sequence values            */
    static size_t capacity = 0;       /* allocated array size       */
    static idx_t next_needed = 0;     /* first index not yet set    */

    /* Initialise cache on first call */
    if (cache == 0) {
        size_t init_cap = 32;
        if ((idx_t)init_cap < n + 1)
            init_cap = (size_t)(n + 1);

#ifdef __cplusplus
        cache = new seq_t[init_cap];
#else
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
#endif
        capacity = init_cap;

        /* Base values up to index 29 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        for (idx_t i = 2; i < 30; ++i) {
            cache[i] = cache[i - 1] * (seq_t)3;
        }

        next_needed = 30; /* next index to compute via recurrence */
    }

    /* Grow the cache if the requested index is beyond current capacity */
    if ((idx_t)capacity < n + 1) {
        size_t newcap = capacity;
        if (newcap == 0) newcap = 1;
        while ((idx_t)newcap < n + 1) newcap <<= 1;

#ifdef __cplusplus
        seq_t *tmp = new seq_t[newcap];
        for (size_t i = 0; i < capacity; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
#endif
        capacity = newcap;
    }

    /* Compute any missing terms up to n using the recurrence */
    if (n >= next_needed) {
        for (idx_t i = next_needed; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 29; ++k)
                sum += cache[i - k] * (seq_t)2;
            sum -= cache[i - 30] * (seq_t)3;
            cache[i] = sum;
        }
        next_needed = n + 1;
    }

    return cache[n];
}
