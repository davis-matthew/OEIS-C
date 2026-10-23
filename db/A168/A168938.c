/*
 * A168938  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(1)=21; a(n)=21*20^(n‑1) for 1≤n≤21 (i.e. multiply the
 *            previous term by 20); a(n)=19*(a(n-1)+…+a(n-21)) - 190*a(n-22)
 *            for n≥22
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements an O(n) computation with simple recurrence and a
   memoised cache that works for any call order. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed values. */
    static seq_t *cache  = NULL;
    static idx_t  capacity = 0;   /* allocated length (≥ last index + 1)     */
    static size_t last_computed = 0; /* maximum index that has been filled   */

    /* Ensure storage for indices 0..n. */
    if (capacity <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        /* Initialise newly allocated slots to 0. */
        for (idx_t i = capacity; i < new_cap; ++i) cache[i] = 0;
        capacity = new_cap;
    }

    /* Initialise a[0] if it has not yet been set. */
    if (capacity > 0 && cache[0] == 0) {
        cache[0] = 1;           /* a(0)=1 */
        /* The first computed index is at least 0. */
        if (last_computed < 0) last_computed = 0; /* safety */
    }

    /* Compute any missing values up to n (in increasing order). */
    if ((idx_t)last_computed < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1) {
                val = 21;                                 /* a(1) */
            } else if (i <= 21) {
                val = cache[i - 1] * 20;                  /* a(i)=a(i-1)*20 */
            } else {            /* i ≥ 22 */
                seq_t sum = 0;
                for (idx_t j = 1; j <= 21; ++j) sum += cache[i - j];
                val = sum * 19 - cache[i - 22] * 190;
            }
            cache[i] = val;
        }
        last_computed = n;
    }

    return cache[n];
}
