/*
 * A170250  Number of reduced words of length n in Coxeter group on 49
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A170250: Number of reduced words of length n in the Coxeter
 *  group on 49 generators with relations S_i^2 = (S_i S_j)^40 = I.
 *
 *  The rational generating function is
 *
 *      (t^40 + 2t^39 + 2t^38 + … + 2t + 1) /
 *      (1128t^40 - 47t^39 - 47t^38 - … - 47t + 1).
 *
 *  From the functional equation  D(t)·G(t) = N(t) one obtains the linear
 *    recurrence (for all n ≥ 0):
 *
 *        a(n) = 47 · Σ_{k=1}^{min(39,n)} a(n-k)          + c(n)
 *                                      - 1128·a(n-40)      (only when n ≥ 40)
 *
 *    where   c(n) = 1   if n = 0 or n = 40,
 *           c(n) = 2   if 1 ≤ n ≤ 39,
 *           c(n) = 0   otherwise.
 *
 *  This implementation uses that recurrence to compute the first n+1 terms
 *  on demand and caches them for subsequent calls.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values of the sequence. */
    static seq_t *cache   = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity (number of slots)   */
    static idx_t cache_size = 0;  /* number of values actually stored       */

    /* Ensure we have room for indices up to n. */
    if ((idx_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute every missing term up to n (inclusive). */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        idx_t m = (i < 39 ? i : 39);      /* how many previous terms to include   */
        for (idx_t j = 1; j <= m; ++j) {
            sum += cache[i - j];
        }

        if (i == 0) {
            cache[0] = 1;
        } else if (i <= 39) {
            cache[i] = 47 * sum + 2;                /* c(n) = 2            */
        } else if (i == 40) {
            cache[40] = 47 * sum - 1128 * cache[0] + 1; /* c(n) = 1, subtract 1128*a(0) */
        } else { /* i >= 41 */
            cache[i] = 47 * sum - 1128 * cache[i - 40];
        }
    }

    cache_size = n + 1;
    return cache[n];
}
