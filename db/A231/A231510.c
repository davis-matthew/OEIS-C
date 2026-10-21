/*
 * A231510  Number of nX3 0..1 arrays with no element less than a strict
 *          majority of its horizontal, vertical, diagonal and antidiagonal
 *          neighbors.
 * https://oeis.org/A231510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4, a(2)=14, a(3)=78, a(4)=343, a(5)=1537, a(6)=7505,
 *            a(7)=35872, a(8)=168887, a(9)=800573, a(10)=3806573,
 *            a(11)=18062200, a(12)=85666516, a(13)=406536978,
 *            a(14)=1929318800, a(15)=9154739800, a(16)=43440293805,
 *            a(17)=206136096165; for n > 17: a(n) = 4*a(n‑1) + 19*a(n‑3) +
 *            4*a(n‑4) - 56*a(n‑5)...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 18 entries: a(0) unused, a(1)..a(17) */
    static const seq_t init[18] = {
        0ULL,
        4ULL,
        14ULL,
        78ULL,
        343ULL,
        1537ULL,
        7505ULL,
        35872ULL,
        168887ULL,
        800573ULL,
        3806573ULL,
        18062200ULL,
        85666516ULL,
        406536978ULL,
        1929318800ULL,
        9154739800ULL,
        43440293805ULL,
        206136096165ULL
    };

    /* Static cache with 1‑based indexing */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity (number of elements) */
    static idx_t computed_max = 0;/* highest index computed and stored in cache */

    /* Fast return for the first 17 terms */
    if (n <= 17)
        return init[n];

    /* Ensure cache large enough */
    if (cache_cap < n + 1) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp;
        if (cache == NULL) {
            tmp = (seq_t *)calloc(new_cap, sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
        /* Install the first 17 values into the cache */
        for (idx_t i = 1; i <= 17 && i < cache_cap; ++i)
            cache[i] = init[i];
        if (computed_max < 17) computed_max = 17;
    }

    /* Return cached value if already computed */
    if (n <= computed_max)
        return cache[n];

    /* Compute values up to n, starting from max(computed_max+1, 18) */
    idx_t start = (computed_max < 17) ? 18 : computed_max + 1;
    for (idx_t i = start; i <= n; ++i) {
        seq_t val =  (seq_t) 4 * cache[i-1]
                   + (seq_t)19 * cache[i-3]
                   + (seq_t) 4 * cache[i-4]
                   + (seq_t)91 * cache[i-9]
                   + (seq_t)105 * cache[i-11]
                   + (seq_t)153 * cache[i-12]
                   + (seq_t) 75 * cache[i-13]
                   + (seq_t) 98 * cache[i-14]
                   + (seq_t) 21 * cache[i-15]
                   + (seq_t) 28 * cache[i-16]
                   - (seq_t)56 * cache[i-5]
                   - (seq_t)49 * cache[i-6]
                   - (seq_t)44 * cache[i-7]
                   - (seq_t)93 * cache[i-8]
                   - (seq_t)11 * cache[i-10];
        cache[i] = val;
    }

    computed_max = n;
    return cache[n];
}
