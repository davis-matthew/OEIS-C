/*
 * A169435  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 36 × (∑_{k=1}^{31} a(n‑k)) − 666 × a(n‑32) + c_n where
 *            c_0 = 1, c_1…c_31 = 2, c_32 = 1, and c_n = 0 for n ≥ 33
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Persistent cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t  len   = 0;   /* number of filled entries */
    static idx_t  cap   = 0;   /* allocated capacity */

    /* Initialise the cache if this is the first call */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure – return 0 */
        cache[0] = (seq_t)1;  /* a(0) = 1 */
        len = 1;
    }

    /* If the requested index is already computed, return it */
    if (n < len)
        return cache[n];

    /* Grow the cache to accommodate n, if necessary */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n)
            new_cap <<= 1;          /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            free(cache);
            cache = NULL;
            return 0;               /* allocation failure */
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Compute terms up to index n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t sum = 0;

        /* sum of the previous up to 31 terms */
        for (idx_t k = 1; k <= 31 && k <= i; ++k)
            sum += cache[i - k];

        seq_t val = 36 * sum;

        if (i >= 32)
            val -= 666 * cache[i - 32];

        /* add the constant c_i */
        if (i == 0 || i == 32)
            val += 1;
        else if (i <= 31)
            val += 2;

        cache[i] = val;
    }

    len = n + 1;
    return cache[n];
}
