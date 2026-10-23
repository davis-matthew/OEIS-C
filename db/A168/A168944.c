/*
 * A168944  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n + 25·a(n-1)+⋯+25·a(n-21) – 325·a(n-22), where p_0 =
 *            1, p_n = 2 for 1 ≤ n ≤ 21, p_22 = 1, and p_n = 0 for n > 22.
 *            (Recurrence derived from the rational generating function for
 *            A168944.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to enable memoisation of previously computed values */
    static seq_t *cache = NULL;   /* array of a(0) … a(last_computed) */
    static idx_t cache_len = 0;   /* number of elements currently stored */

    if (n < cache_len)           /* already computed */
        return cache[n];

    /* extend the cache to accommodate a(n) */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)                      /* allocation failure – abort */
        return (seq_t)0;
    cache = tmp;

    /* compute missing terms sequentially */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;

        /* add the polynomial term p_i */
        if (i <= 22) {
            sum = (i == 0 || i == 22) ? (seq_t)1 : (seq_t)2;
        }

        /* add 25·a(i-j) for j = 1 .. 21 (as far as defined) */
        for (idx_t j = 1; j <= 21 && i >= j; ++j)
            sum += (seq_t)25 * cache[i - j];

        /* subtract 325·a(i-22) when i ≥ 22 */
        if (i >= 22)
            sum -= (seq_t)325 * cache[i - 22];

        cache[i] = sum;
    }

    cache_len = new_len;
    return cache[n];
}
