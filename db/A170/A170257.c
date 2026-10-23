/*
 * A170257  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence forming the generating function of A170257:
   a(0) = 1
   For 1 ≤ n ≤ 40:        a(n) = 2 + 6 * Σ_{i=0}^{n-1} a(i)
   a(41) = 1 + 6 * Σ_{i=1}^{40} a(i) - 21 * a(0)
   For n ≥ 42:           a(n) = 6 * Σ_{k=1}^{40} a(n-k) - 21 * a(n-41)
   Equivalent rational generating function:
   (t^41 + 2*t^40 + … + 2*t + 1) / (21*t^41 - 6*t^40 - … - 6*t + 1)
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* stored terms a(0)…a(len-1) */
    static size_t cap = 0;           /* allocated capacity of cache    */
    static size_t len = 0;           /* number of terms currently stored */
    static seq_t last40_sum = (seq_t)0;   /* sum of the last 40 terms        */
    static seq_t cum_sum = (seq_t)0;      /* sum of all previous terms       */

    if (n >= len) {                /* extend cache up to index n       */
        size_t new_cap = cap ? (cap << 1) : 8;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        if (new_cap > cap) {       /* reallocate and zero new slots    */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            for (size_t i = cap; i < new_cap; ++i)
                tmp[i] = (seq_t)0;
            cache = tmp;
            cap = new_cap;
        }

        for (idx_t i = len; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)1;
            } else if (i <= 40) {
                val = (seq_t)2 + (seq_t)6 * cum_sum;
            } else if (i == 41) {
                val = (seq_t)1 + (seq_t)6 * last40_sum - (seq_t)21 * cache[0];
            } else { /* i >= 42 */
                val = (seq_t)6 * last40_sum - (seq_t)21 * cache[i - 41];
            }

            cache[i] = val;

            /* update helpers for the next iteration */
            cum_sum += val;
            last40_sum += val;
            if (i >= 40)
                last40_sum -= cache[i - 40];   /* keep at most 40 terms */
            ++len;
        }
    }

    return cache[(size_t)n];
}
