/*
 * A170096  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The coefficients a(n) satisfy the linear recurrence obtained
 *            from the rational generating function P(t) = t^37 + 2*t^36 +
 *            2*t^35 + ... + 2*t + 1 Q(t) = 703*t^37 - 37*t^36 - 37*t^35 - ...
 *            - 37*t^1 + 1 namely a(0) = 1 a(n) = p_n + 37 *
 *            Σ_{k=max(0,n-36)}^{n-1} a(k) - 703 * a(n-37) (n ≥ 1) where
 *            p_n...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External linkage required by the problem statement */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* array holding computed a[i] values   */
    static idx_t cap   = 0;       /* current capacity in elements         */
    static idx_t cached = 0;      /* number of computed elements (<=cap)  */

    /* Ensure we have space for values up to index n */
    if (n + 1 > cap)
    {
        /* Grow to next power of two at least n+1 */
        size_t new_cap = (cap == 0) ? 64 : cap;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* On allocation failure we simply exit with 0 (unlikely in tests) */
        if (tmp == NULL)
            return 0;
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the very first term if needed */
    if (cached == 0 && n >= 0)
    {
        cache[0] = 1;   /* a(0) = 1 */
        cached   = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t k = cached; k <= n; ++k)
    {
        if (k == 0)
        {
            /* already handled above */
            continue;
        }

        /* Compute sum_{i=max(0,k-36)}^{k-1} a[i] */
        seq_t sum_small = 0;
        idx_t start = (k > 36) ? (k - 36) : 0;
        for (idx_t j = start; j < k; ++j)
        {
            sum_small += cache[j];
        }

        /* p_k: 2 for 1≤k≤36, 1 for k=37, else 0 */
        seq_t p_k;
        if (k <= 36)
            p_k = 2;
        else if (k == 37)
            p_k = 1;
        else
            p_k = 0;

        /* Recurrence: a(k) = p_k + 37 * sum_small - 703 * a(k-37) (if k≥37) */
        seq_t term = p_k + (seq_t)37 * sum_small;
        if (k >= 37)
            term -= (seq_t)703 * cache[k - 37];

        cache[k] = term;
    }

    cached = n + 1;  /* we have computed up to index n */
    return cache[n];
}
