/*
 * A169120  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^26 = I.
 * https://oeis.org/A169120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*Σ_{k=1}^{25} a(n-k) - 45*a(n-26) + p(n) with p(0)=1,
 *            p(26)=1, p(n)=2 for 1≤n≤25, and p(n)=0 otherwise
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of already computed values.  The cache is enlarged on demand. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        /* Grow the cache to hold entries up to n. */
        idx_t newsize = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        cache = tmp;

        /* Compute values that have not been seen yet. */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t j = 1; j <= 25 && i >= j; ++j) {
                sum += cache[i - j] * (seq_t)9;   /* 9·a(i-j) */
            }

            seq_t term45 = 0;
            if (i >= 26) term45 = cache[i - 26] * (seq_t)45;   /* 45·a(i-26) */

            seq_t p = 0;
            if (i == 0 || i == 26) {
                p = 1;
            } else if (i <= 25) {   /* 1 ≤ i ≤ 25 */
                p = 2;
            }

            cache[i] = sum - term45 + p;   /* recurrence */
        }
        cache_size = newsize;
    }
    return cache[n];
}
