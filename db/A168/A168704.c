/*
 * A168704  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^17 = I.
 * https://oeis.org/A168704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A168704: number of reduced words of length n in the Coxeter group
 * with 27 generators S_i and relations (S_i)^2 = (S_i S_j)^17 = I.
 *
 * The generating function is
 *   (1 + 2t + 2t^2 + … + 2t^16 + t^17) /
 *   (1 – 25t – 25t^2 – … – 25t^16 + 325t^17).
 * Hence the coefficients satisfy the linear recurrence
 *
 *   a(n) - 25 a(n-1) - … - 25 a(n-16) + 325 a(n-17) = p_n
 *
 * with p_0 = p_17 = 1, p_k = 2 for 1 ≤ k ≤ 16, and p_k = 0 for k > 17.
 * The initial terms a(0)=1, a(1)=27, a(2)=702, … are obtained from this recurrence.
 *
 * This implementation uses memoised linear computation up to the maximum
 * request `n`.  It is valid for all idx_t/seq_t types used by the build
 * system and is safe for C11 and C++17.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache   = NULL;   /* array of a(i) for i = 0 … cache_len-1 */
    static idx_t  cache_len = 0;     /* number of values stored in cache */

    /* Resize cache to hold terms up to n (if necessary) */
    if (cache_len <= n)
    {
        size_t new_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* Compute new terms sequentially */
        for (idx_t cur = cache_len; cur <= n; ++cur)
        {
            seq_t sum = 0;  /* accumulator for the recurrence RHS without p_cur */

            /* 25·a(cur-1) + … + 25·a(cur-16) */
            for (int j = 1; j <= 16; ++j)
            {
                if (cur >= j)
                    sum += ((seq_t)25) * cache[cur - j];
            }

            /* – 325·a(cur-17) */
            if (cur >= 17)
                sum -= ((seq_t)325) * cache[cur - 17];

            /* Add the inhomogeneous term p_cur */
            seq_t p = 0;
            if (cur == 0 || cur == 17)
                p = 1;
            else if (cur <= 16)
                p = 2;
            /* else p remains 0 for cur > 17 */

            cache[cur] = sum + p;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
