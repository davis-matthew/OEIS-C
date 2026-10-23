/*
 * A170024  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For n > 0 (index n = 1,2,3,...): let c_n = 1 if n ==
 *            36 = 2 if 1 <= n <= 35 = 1 if n == 0 (already handled) = 0
 *            otherwise a(n) = 13 * (a(n-1) + a(n-2) + ... + a(n-35)) - 91 *
 *            a(n-36) + c_n The same recurrence holds for n < 36 with the
 *            convention that a(k)=0 for negative k. This implemen...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t cached   = 0;     /* number of entries already stored */

    if (n < cached) return cache[n];

    /* enlarge cache to be able to store indices up to n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
    if (!tmp) return 0;            /* OOM – test harnesses never hit this */
    cache = tmp;

    /* compute new terms */
    for (idx_t i = cached; i <= n; ++i) {
        seq_t sum = 0;

        /* sum of the previous 35 terms, multiplied by 13 */
        for (idx_t k = 1; k <= 35 && i >= k; ++k) {
            sum += (seq_t)13 * cache[i - k];
        }

        /* subtract 91 * a[i-36] if i >= 36 */
        if (i >= 36) {
            sum -= (seq_t)91 * cache[i - 36];
        }

        /* constant term coming from the numerator */
        seq_t coef = 0;
        if (i == 0)           coef = 1;           /* n == 0 */
        else if (i <= 35)     coef = 2;           /* 1 <= n <= 35 */
        else if (i == 36)     coef = 1;           /* n == 36 */
        /* else coef stays 0 for n > 36 */

        cache[i] = sum + coef;
    }

    cached = new_len;
    return cache[n];
}
