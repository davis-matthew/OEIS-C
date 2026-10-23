/*
 * A168783  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^19 = I.
 * https://oeis.org/A168783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*(a(n-1)+…+a(n-18)) - 36*a(n-19) + p_n where p_0 = 1,
 *            p_1,…,p_18 = 2, p_19 = 1, and p_n = 0 for n > 19
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache   = NULL;   /* cache[0 .. cache_len-1] */
    static idx_t cache_len = 0;     /* number of terms in cache   */
    static idx_t cache_cap = 0;     /* allocated capacity         */

    /* Ensure the cache can hold up to n */
    if (n + 1 > cache_cap) {
        idx_t new_cap;
        if (cache_cap == 0) new_cap = 16;
        else                new_cap = cache_cap << 1;

        while (new_cap < n + 1) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;          /* allocation failure → zero */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached value if already known */
    if (n < cache_len) return cache[n];

    /* Compute new terms from cache_len up to n (inclusive) */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* sum of the last up to 18 terms preceding i */
        idx_t maxk = (i < 18) ? i : 18;
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= maxk; ++k) {
            sum += cache[i - k];
        }

        /* 8 * sum */
        seq_t val = sum * (seq_t)8;

        /* subtract 36*a(i-19) if needed */
        if (i >= 19) {
            val -= cache[i - 19] * (seq_t)36;
        }

        /* add the p_n contribution */
        seq_t pn = (seq_t)0;
        if (i == 0 || i == 19)     pn = (seq_t)1;
        else if (i <= 18)          pn = (seq_t)2;

        val += pn;

        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
