/*
 * A170696  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A170696:
   a(0) = 1.
   For n ≥ 1:
       a(n) = 13 * Σ_{k=1}^{49} a(n−k) – 91 * a(n−50) + p_n
   where the non‑homogeneous part is:
       p_0 = 1,
       p_n = 2 for 1 ≤ n ≤ 49,
       p_50 = 1,
       p_n = 0 for n > 50. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cache[i] holds a(i) for i < max_cached */
    static idx_t max_cached = 0;    /* number of terms currently stored */

    if (n >= max_cached) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* out of memory sentinel */

        /* copy existing cached values */
        for (idx_t i = 0; i < max_cached; ++i)
            new_cache[i] = cache[i];

        /* fill new entries */
        for (idx_t i = max_cached; i <= n; ++i) {
            seq_t sum_prev = 0;
            /* sum of a[i−1] .. a[i−49] with indices ≥ 0 */
            for (idx_t k = 1; k <= 49; ++k) {
                if (i >= k)
                    sum_prev += new_cache[i - k];
            }

            seq_t term = sum_prev * (seq_t)13;

            if (i >= 50)
                term -= new_cache[i - 50] * (seq_t)91;

            /* add the non‑homogeneous part p_i */
            seq_t p_val = 0;
            if (i == 0)            p_val = (seq_t)1;
            else if (i <= 49)     p_val = (seq_t)2;
            else if (i == 50)     p_val = (seq_t)1;
            /* p_val remains 0 for i > 50 */

            new_cache[i] = term + p_val;
        }

        free(cache);
        cache = new_cache;
        max_cached = n + 1;
    }

    return cache[n];
}
