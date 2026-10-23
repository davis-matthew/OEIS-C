/*
 * A170166  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(0) = 1
   For 1 ≤ n ≤ 38 : a(n) = 11·∑_{k=1}^{n} a(n–k) + 2
   a(39)          : a(39) = 11·∑_{k=1}^{38} a(39–k) – 66·a(0) + 1
   For n ≥ 40    : a(n) = 11·∑_{k=1}^{38} a(n–k) – 66·a(n–39)   (homogeneous part)
   All lower‑index terms are treated as zero. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Dynamic cache of already computed terms */
    static seq_t *cache      = NULL;   /* array of computed terms */
    static idx_t cache_cap   = 0;      /* allocated capacity (number of slots) */
    static idx_t cache_used  = 0;      /* number of terms currently stored */

    /* Resize the cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache,
                                      (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory – return 0 as sentinel */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_used; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else {
            seq_t sum = (seq_t)0;

            /* 11 * sum_{k=1}^{38} a(i-k)  (terms with negative index are 0) */
            for (int k = 1; k <= 38; ++k) {
                if (i >= (idx_t)k)
                    sum = sum + (seq_t)(11) * cache[i - k];
            }

            /* subtract 66·a(i-39) if i ≥ 39 */
            if (i >= 39) {
                sum = sum - (seq_t)(66) * cache[i - 39];
            }

            /* add the non‑homogeneous part */
            seq_t c = (seq_t)0;
            if (i <= 38)        c = (seq_t)2;
            else if (i == 39)   c = (seq_t)1;
            /* for i ≥ 40, c stays zero */

            val = sum + c;
        }
        cache[i] = val;
    }

    cache_used = n + 1;
    return cache[n];
}
