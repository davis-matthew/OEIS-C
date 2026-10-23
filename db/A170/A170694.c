/*
 * A170694  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For 1 ≤ n ≤ 49: a(n) = 13 * 12^(n-1). For n ≥ 50: a(n)
 *            = a(n-50) - (1/6) * Σ_{i=1}^{49} a(n-i)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoized values */
    static idx_t  cap   = 0;     /* allocated size */
    static idx_t  last  = 0;     /* highest index computed */

    /* Allocate initial space if needed */
    if (!cache) {
        cap = 64;                     /* start with some space */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        last = 0;
    }

    /* Grow cache to accommodate n */
    if (n >= cap) {
        size_t new_cap = n + 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute new terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)13;
        } else if (i < 50) {          /* geometric progression */
            cache[i] = cache[i-1] * (seq_t)12;
        } else {                      /* recurrence uses 49 previous terms */
            seq_t sum = (seq_t)0;
            /* sum of the previous 49 terms */
            for (idx_t k = 1; k <= 49; ++k) {
                sum += cache[i - k];
            }
            cache[i] = cache[i - 50] - sum / (seq_t)6;
        }
    }
    last = n;
    return cache[n];
}
