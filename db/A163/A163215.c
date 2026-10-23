/*
 * A163215  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=32, a(2)=992, a(3)=30752, a(4)=952816 a(n) =
 *            30*(a(n-1)+a(n-2)+a(n-3)) - 465*a(n-4) for n >= 5
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache and bookkeeping */
    static seq_t *cache      = NULL;   /* array of computed terms   */
    static size_t cache_cap = 0;       /* allocated capacity       */
    static idx_t  max_n      = 0;       /* highest index already computed */

    /* first call: initialise cache with at least the first six terms */
    if (cache == NULL) {
        /* ensure room for the requested index and a few more if needed */
        size_t need = (size_t)(n + 1);
        if (need < 5) need = 5;         /* need indices 0..4 */
        if (need < 16) need = 16;        /* small default size */
        cache = (seq_t *)calloc(need, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failed */

        cache_cap = need;
        /* set the known initial terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)32;
        cache[2] = (seq_t)992;
        cache[3] = (seq_t)30752;
        cache[4] = (seq_t)952816;
        max_n    = 4;
    }

    /* grow the cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* if the value is already known, return it */
    if (n <= max_n) {
        return cache[n];
    }

    /* compute missing terms up to index n using the recurrence */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t sum = cache[k-1] + cache[k-2] + cache[k-3];
        cache[k] = (30 * sum) - (465 * cache[k-4]);
    }

    max_n = n;
    return cache[n];
}
