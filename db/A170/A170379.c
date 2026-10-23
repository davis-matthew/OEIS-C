/*
 * A170379  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(k) = 2 + 32 * Σ_{i=0}^{k-1} a(i) for 1 ≤ k ≤ 42 a(k)
 *            = 32 * Σ_{i=k-42}^{k-1} a(i) – 528 * a(k-43) for k ≥ 43
 *            (Recurrence derived from the given rational generating
 *            function.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *cache    = NULL;   /* cached values */
    static size_t cache_len = 0;     /* number of terms stored so far */
    static seq_t sumPrev   = 0;      /* Σ_{i=0}^{k-1} a(i) for the next term */
    static seq_t sum42     = 0;      /* Σ_{i=k-42}^{k-1} a(i) for the current term */

    /* Extend cached array to cover index n */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp && new_len > 0) exit(1);  /* allocation failed */
        cache = tmp;

        for (size_t k = cache_len; k <= (size_t)n; ++k) {
            seq_t val;

            if (k == 0) {
                val = (seq_t)1;
            } else if (k <= 42) {
                val = (seq_t)2 + (seq_t)32 * sumPrev;
            } else if (k == 43) {
                /* initialise sum42 for the first 42‑term window (indices 1..42) */
                seq_t tmpSum = sumPrev - cache[0];
                val = (seq_t)32 * tmpSum - (seq_t)528 * cache[0];
                sum42 = tmpSum;
            } else { /* k > 43 */
                val = (seq_t)32 * sum42 - (seq_t)528 * cache[k - 43];
            }

            cache[k] = val;
            sumPrev += val;

            if (k >= 44) {
                /* shift the 42‑term window: add new term, remove old term */
                sum42 += val - cache[k - 43];
            }
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
