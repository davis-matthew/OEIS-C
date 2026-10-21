/*
 * A228706  Expansion of (1 - 3*x + 5*x^2 - 3*x^3 + x^4)/((1-x)^4*(1+x^2)^2).
 * https://oeis.org/A228706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 8*a(n-2) + 12*a(n-3) - 14*a(n-4) + 12*a(n-5) -
 *            8*a(n-6) + 4*a(n-7) - a(n-8) a(0)=1, a(1)=1, a(2)=1, a(3)=5,
 *            a(4)=11, a(5)=14, a(6)=18, a(7)=30
 * Range:     u8 n <= 16, u16 n <= 114, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache     = NULL;
    static idx_t  cache_len = 0;

    /* Ensure cache is large enough and pre‑initialise the first 8 terms */
    if (cache_len <= n) {
        size_t new_len = (size_t)((n + 1) > 8 ? (n + 1) : 8);
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure, behaviour undefined */

        /* initialise first 8 terms if not already done */
        if (cache_len < 8) {
            static const seq_t init[8] = {1, 1, 1, 5, 11, 14, 18, 30};
            size_t i;
            for (i = cache_len; i < 8 && i < new_len; ++i)
                tmp[i] = init[i];
        }

        /* compute remaining terms up to n */
        for (idx_t i = ((cache_len > 8) ? cache_len : 8); i <= n; ++i) {
            tmp[i] = 4*tmp[i-1] - 8*tmp[i-2] + 12*tmp[i-3] - 14*tmp[i-4]
                     + 12*tmp[i-5] - 8*tmp[i-6] + 4*tmp[i-7] - tmp[i-8];
        }

        cache     = tmp;
        cache_len = (idx_t)new_len;
    }

    return cache[n];
}
