/*
 * A170296  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies * a(0) = 1 * for 1 ≤ n ≤ 40: a(n) = 2 + 45 *
 *            Σ_{k=0}^{n-1} a(k) * for n ≥ 41: a(n) = 45 * Σ_{k=1}^{40} a(n-k)
 *            - 1035 * a(n-41) + (n==41 ? 1 : 0) * This recurrence follows
 *            from the rational generating function in the OEIS entry
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free, size_t */

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;      /* array of computed a(i) */
    static size_t cap = 0;           /* allocated size of cache */
    static size_t len = 0;           /* number of terms currently in cache (next free index) */
    static seq_t cum = 0;            /* cumulative sum of a(0) .. a(len-1) */
    static seq_t sum40 = 0;          /* sum of the last 40 terms, i.e. a(len-40) .. a(len-1) */

    /* Ensure the cache can hold indices up to n. */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap * 2 : 64;
        if ((size_t)n + 1 > newcap)
            newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)    /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        cache = tmp;
        cap = newcap;
    }

    /* Initialise cache on first call. */
    if (len == 0) {
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cum     = cache[0];
        len     = 1;
    }

    /* Compute missing terms up to index n. */
    for (idx_t i = len; i <= n; ++i) {
        seq_t term;
        if (i <= 40) {
            /* a(i) = 2 + 45 * cum (sum of a(0)..a(i-1)) */
            term = (seq_t)2 + (seq_t)45 * cum;
            cache[i] = term;
            cum += term;
            if (i == 40)          /* initialise sum of last 40 terms after computing a(40) */
                sum40 = cum - cache[0];   /* sum of a(1)..a(40) */
        } else { /* i ≥ 41 */
            seq_t add_one = (i == 41) ? (seq_t)1 : (seq_t)0;
            term = (seq_t)45 * sum40 - (seq_t)1035 * cache[i - 41] + add_one;
            cache[i] = term;
            /* Update sliding window of the last 40 terms: sum of a(i-39)..a(i). */
            sum40 = sum40 - cache[i - 40] + term;
            cum  += term;
        }
        ++len;
    }

    return cache[n];
}
