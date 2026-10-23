/*
 * A166877  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A166877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; For n≥1, a(n)=4·∑_{i=1}^{12}a(n-i) −10·a(n-13)+c_n where
 *            c_n=1 if n==0 or n==13, 2 if 1≤n≤12, otherwise 0. This
 *            recurrence follows from the generating function:
 *            G(t)=(t^13+2t^12+⋯+2t+1)/(10t^13−4t^12−⋯−4t+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;     /* number of elements allocated */
    static idx_t  computed   = 0;     /* largest index already filled */

    /* Resize array if necessary */
    if (n+1 > cache_size) {
        idx_t new_size = n+1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp && new_size > 0) { /* allocation failed */
            return 0; /* or handle error appropriately */
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute terms up to index n */
    if (n >= computed) {
        for (idx_t k = computed; k <= n; ++k) {
            seq_t term = 0;

            /* constant part from the numerator */
            if (k == 0)          term += 1;          /* p_0 */
            else if (k == 13)    term += 1;          /* p_13 */
            else if (k <= 12)    term += 2;          /* p_k for 1≤k≤12 */

            /* 4 * sum_{i=1}^{12} a(k-i) */
            seq_t sum = 0;
            for (idx_t i = 1; i <= 12; ++i) {
                if (k >= i) sum += 4 * cache[k - i];
            }

            /* -10 * a(k-13) when k≥13 */
            if (k >= 13) sum -= 10 * cache[k - 13];

            cache[k] = sum + term;
        }
        computed = n + 1;
    }

    return cache[n];
}
