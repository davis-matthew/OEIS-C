/*
 * A169037  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 * For n ≥ 1: * let S(n) = Σ_{k=1}^{min(n,23)} a(n-k) *
 *            a(n) = 22·S(n) – (n ≥ 24 ? 253·a(n-24) : 0) + B(n) * where B(0)
 *            = 1, B(1…23) = 2, B(24) = 1, B(n>24) = 0 * * This recurrence
 *            follows from the rational generating function *
 *            (t^24+2t^23+…+2t+1)/(253t^24–22t^23–…–22t–1). The code memoi...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* first free index */
    static idx_t max_computed = 0; /* last computed index (inclusive) */

    /* initialise cache with a(0) */
    if (cache == NULL) {
        cache_size = 4;  /* start with a small capacity */
        cache     = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0]   = 1;   /* a(0) = 1 */
        max_computed = 0;
    }

    /* grow the array if needed */
    if (cache_size <= n) {
        size_t new_size = cache_size * 2;
        if (new_size <= n) new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* compute new values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t limit = (i < 23) ? i : 23;   /* min(i,23) */
        for (idx_t j = 1; j <= limit; ++j)
            sum += cache[i - j];

        seq_t val = 22 * sum;                /* 22·S(i)              */
        if (i >= 24)
            val -= 253 * cache[i - 24];       /* –253·a(i−24)          */

        /* add the B(i) contribution */
        if (i == 0)
            val = 1;                         /* a(0) explicitly set */
        else if (i <= 23)
            val += 2;                        /* B(i)=2 for 1≤i≤23   */
        else if (i == 24)
            val += 1;                        /* B(24)=1              */

        cache[i] = val;
    }
    max_computed = (n > max_computed) ? n : max_computed;

    return cache[n];
}
