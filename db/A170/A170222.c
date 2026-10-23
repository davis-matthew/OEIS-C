/*
 * A170222  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19·∑_{k=1}^{39} a(n-k) – 190·a(n-40) + b(n), where
 *            b(0)=1, b(1…39)=2, b(40)=1, and b(n)=0 for n>40. This follows
 *            from the generating function (1+2∑_{k=1}^{39}t^k + t^40)/(1 –
 *            19∑_{k=1}^{39}t^k + 190t^40)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* External types seq_t and idx_t are provided by the build. */

seq_t a(idx_t n)
{
    /* Memoised sequence cache. */
    static seq_t *cache   = NULL;
    static idx_t cached_size = 0;   /* next free index; cache holds 0 … cached_size-1 */

    if (n >= cached_size) {
        /* Expand the cache to hold the required terms. */
        size_t new_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

        /* Compute each new term sequentially. */
        for (idx_t i = cached_size; i <= n; ++i) {
            seq_t sum = 0;

            /* Sum the last 39 values a[i-1], …, a[max(i-39,0)]. */
            idx_t start = (i >= 39) ? (i - 39) : 0;
            for (idx_t k = start; k < i; ++k)
                sum += cache[k];

            seq_t val = (seq_t)19 * sum;
            if (i >= 40)
                val -= (seq_t)190 * cache[i - 40];

            /* Add the constant term from the numerator of the G.F. */
            if (i == 0)
                val += 1;           /* b(0) = 1 */
            else if (i <= 39)
                val += 2;           /* b(1…39) = 2 */
            else if (i == 40)
                val += 1;           /* b(40) = 1 */

            cache[i] = val;
        }
        cached_size = n + 1;
    }

    return cache[n];
}
