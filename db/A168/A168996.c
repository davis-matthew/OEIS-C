/*
 * A168996  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 29 * Σ_{k=1}^{22} a(n-k) – 435 * a(n-23) + p_n where p_0
 *            = 1, p_1 … p_22 = 2, p_23 = 1, p_n = 0 for n > 23
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* table of computed values   */
    static size_t  cap    = 0;      /* current capacity of cache  */
    static idx_t   last   = 0;      /* largest index that has been
                                       computed (0 means only a(0)      */

    /* make sure we have enough storage */
    if ((size_t)n + 1 > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;          /* out of memory : fall back to 0 */
        cache = tmp;
        cap   = n + 1;
    }

    /* initialise the zeroth term (only once) */
    if (cache[0] == 0) cache[0] = 1;

    /* compute terms that have not yet been computed */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = 0;

        /* 29 * sum_{k=1}^{22} a(i-k)  ,  only counting existing terms */
        for (idx_t k = 1; k <= 22 && i >= k; ++k) {
            sum += 29 * cache[i - k];
        }

        seq_t val = sum;

        /* subtract 435 * a(i-23) when i ≥ 23 */
        if (i >= 23) {
            val -= 435 * cache[i - 23];
        }

        /* add the constant term p_i */
        if (i == 0) {
            val += 1;
        } else if (i <= 22) {
            val += 2;
        } else if (i == 23) {
            val += 1;
        }

        cache[i] = val;
    }

    last = n;
    return cache[n];
}
