/*
 * A170082  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 23*(a(n-1)+⋯+a(n-36)) - 276*a(n-37) for n>37 a(0)=1,
 *            a(1)=25, …, a(37) are obtained from the generating function. The
 *            recurrence is implemented modulo the integer type seq_t, with
 *            all intermediate results staying within seq_t
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache    = NULL;   /* hold computed values    */
    static size_t cache_cap = 0;       /* current allocated size */
    static idx_t  cache_len = 0;       /* number of values computed */

    /* make sure there is room for index n */
    ensure_cache(n, &cache, &cache_cap);

    /* initialise first term */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    const seq_t C23  = (seq_t)23;
    const seq_t C276 = (seq_t)276;

    for (idx_t i = cache_len; i <= n; ++i) {
        /* sum of the previous 36 values (or fewer if i<36) */
        seq_t sum = 0;
        idx_t max_j = i < 36 ? i : 36;
        for (idx_t j = 1; j <= max_j; ++j)
            sum += cache[i - j];

        /* recurrence part */
        seq_t val = sum * C23;
        if (i >= 37)
            val -= cache[i - 37] * C276;

        /* add the numerator term for i <= 37  (0:1, 1..36:2, 37:1) */
        if (i <= 37) {
            seq_t Ni = (i == 0) ? (seq_t)1
                                : ((i <= 36) ? (seq_t)2 : (seq_t)1);
            val += Ni;
        }

        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
