/*
 * A170680  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 45*Σ_{i=1}^{48} a(n-i) – 1035*a(n-49) + p(n) where p(n) =
 *            1 if n = 0 or 49, p(n) = 2 if 1 ≤ n ≤ 48, otherwise p(n) = 0.
 *            The recurrence is valid for all n ≥ 0. The first term a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of OEIS sequence A170680. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* memoized values */
    static size_t cap   = 0;       /* capacity of cache array   */
    static idx_t len    = 0;       /* number of initialized terms */

    /* Initial allocation and first term. */
    if (cache == NULL) {
        cap = 64;                       /* start with a modest size */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;            /* a(0) */
        len = 1;
    }

    /* Grow the cache if necessary. */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap * 2 : 64;
        while (new_cap <= (size_t)n + 1)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (new_cache == NULL) { /* allocation failed – abort */
            abort();
        }
        for (size_t i = 0; i < len; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* Return cached value if already computed. */
    if (n < len)
        return cache[n];

    /* Compute terms from len up to n (inclusive). */
    for (idx_t k = len; k <= n; ++k) {
        seq_t sum = (seq_t)0;
        idx_t m = (k < (idx_t)48) ? k : (idx_t)48;   /* up to 48 predecessors */
        for (idx_t i = (idx_t)1; i <= m; ++i)
            sum += cache[k - i];

        seq_t term = sum * (seq_t)45;
        if (k >= (idx_t)49)
            term -= ((seq_t)1035) * cache[k - (idx_t)49];

        seq_t pk;
        if (k == (idx_t)0 || k == (idx_t)49)
            pk = (seq_t)1;
        else if (k < (idx_t)49)
            pk = (seq_t)2;
        else
            pk = (seq_t)0;

        term += pk;
        cache[k] = term;
    }

    len = n + (idx_t)1;
    return cache[n];
}
