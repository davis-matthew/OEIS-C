/*
 * A052949  Expansion of (2-4*x+x^3)/((1-x)*(1-2*x-x^2+x^3)).
 * https://oeis.org/A052949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=2, a(1)=2, a(2)=4, a(n) = 2*a(n-1) + a(n-2) - a(n-3) - 1 (n
 *            ≥ 3)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 55, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* current valid length */

    /* expand cache if necessary */
    if (n >= cache_len)
    {
        /* determine new length (grow at least twice or up to needed size) */
        idx_t new_len = cache_len > 0 ? 2 * cache_len : 1;
        if (new_len < n + 1)
            new_len = n + 1;

        /* reallocate */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp)
            return 0;           /* out of memory; in tests this should not occur */
        cache = tmp;

        /* compute new terms */
        if (cache_len == 0)
            cache_len = 0;          /* just to silence unused var warning */

        if (cache_len == 0) {
            /* store initial values */
            cache[0] = 2;
            cache_len = 1;
            if (n >= 1) {
                cache[1] = 2;
                cache_len = 2;
            }
            if (n >= 2) {
                cache[2] = 4;
                cache_len = 3;
            }
        }
        /* compute remaining terms up to n */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            if (i == 0) {
                cache[i] = 2;
            } else if (i == 1) {
                cache[i] = 2;
            } else if (i == 2) {
                cache[i] = 4;
            } else {
                cache[i] = cache[i-1] * (seq_t)2
                           + cache[i-2]
                           - cache[i-3]
                           - (seq_t)1;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
