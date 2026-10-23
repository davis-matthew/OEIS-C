/*
 * A162889  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=46, a(2)=2070, a(3)=92115. For n ≥ 4 a(n) =
 *            44*a(n−1) + 44*a(n−2) – 990*a(n−3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t  cache_size = 0;   /* allocated capacity */
    static idx_t  max_index  = 0;   /* highest index computed */

    /* constants used in the recurrence */
    static const seq_t C44  = (seq_t)44;
    static const seq_t C990 = (seq_t)990;

    /* initialise the first four terms on first call */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) abort();         /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)46;
        cache[2] = (seq_t)2070;
        cache[3] = (seq_t)92115;
        max_index = 3;
    }

    /* extend the cache if necessary and compute new values */
    if (n > max_index) {
        /* grow array if needed */
        if ((n + 1) > cache_size) {
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
            if (!tmp) abort();
            cache = tmp;
            cache_size = n + 1;
        }
        for (idx_t i = max_index + 1; i <= n; ++i) {
            cache[i] = C44 * cache[i - 1] + C44 * cache[i - 2] - C990 * cache[i - 3];
        }
        max_index = n;
    }

    return cache[(size_t)n];
}
