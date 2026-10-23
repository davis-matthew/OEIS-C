/*
 * A165939  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, and for n<10 a(n) = N​(n) + 21⋅∑_{k=1}^{n} a(n-k), where
 *            N​(0)=1 and N​(1)…N​(9)=2. For n≥10 the homogeneous recurrence
 *            from the generating function holds, a(n) = 21⋅∑_{k=1}^{9} a(n-k)
 *            – 231⋅a(n-10), with an additional +1 for n==10 because N​(10)=1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cached values */
    static size_t cache_len = 0;   /* number of valid entries */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* enlarge cache if necessary */
    if (cache_len <= (size_t)n) {
        size_t need = (size_t)n + 1;
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < need) new_cap <<= 1;

        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        if (cache) {
            memcpy(tmp, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute new entries lazily */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 1;
        } else if (i < 10) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) sum += cache[i - k];
            val = 2 + 21 * sum;
        } else {
            seq_t sum9 = 0;
            for (idx_t k = 1; k <= 9; ++k) sum9 += cache[i - k];
            val = 21 * sum9 - 231 * cache[i - 10];
            if (i == 10) val += 1;  /* N(10)=1 */
        }
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
