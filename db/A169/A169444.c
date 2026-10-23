/*
 * A169444  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 45 * Σ_{k=1}^{31} a(n-k) - 1035 * a(n-32) + b(n)
 *            where b(n) = 1 if n==0 or n==32, b(n) = 2 if 1≤n≤31, b(n) = 0
 *            otherwise. This recurrence follows from the generating function
 *            (1+2∑_{k=1}^{31}x^k+x^32)/(1-45∑_{k=1}^{31}x^k+1035x^32)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ----- static state ------------------------------------------------- */
    static seq_t *cache        = NULL;   /* already computed terms   */
    static size_t cache_len    = 0;       /* number of stored terms   */
    static size_t cache_cap    = 0;       /* allocated capacity       */
    static seq_t sum_last31    = 0;       /* Σ a(i-k) for k=1..31     */

    /* ----- ensure enough space ------------------------------------------ */
    if ((size_t)n >= cache_len) {
        /* grow capacity to next power of two ≥ n+1 */
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* ----- compute missing terms ----------------------------------- */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* sum of the previous 31 terms is held in sum_last31   */
            seq_t term = 45 * sum_last31;

            if (i >= 32) {
                term -= 1035 * cache[(size_t)(i - 32)];
            }

            /* add the numerator contribution b(i) */
            if (i == 0)          term += (seq_t)1;
            else if (i <= 31)    term += (seq_t)2;
            else if (i == 32)    term += (seq_t)1;

            cache[(size_t)i] = term;

            /* update sliding window sum for next iteration */
            sum_last31 += term;
            if (i >= 31) {
                sum_last31 -= cache[(size_t)(i - 31)];
            }
        }

        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
