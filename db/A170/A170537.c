/*
 * A170537  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 46*sum_{k=1}^{45} a(n-k) - 1081*a(n-46) + p(n) where
 *            p(0)=1, p(n)=2 for 1≤n≤45, p(46)=1, and p(n)=0 for n>46
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state: memoised values and sliding‑window sum. */
    static seq_t *cache = NULL;          /* computed a[0] … a[max_computed] */
    static size_t capacity = 0;          /* allocated size of cache              */
    static idx_t  max_computed = (idx_t)-1; /* highest n already computed   */
    static seq_t  sum45 = 0;             /* sum of last up to 45 terms          */

    /* Initialise storage on first call. */
    if (max_computed == (idx_t)-1) {
        capacity = n + 1;
        if (capacity < 48)           /* at least enough for a[0] and the 45‑window */
            capacity = 48;
        cache = (seq_t*)malloc(sizeof(seq_t) * capacity);
        if (!cache) return (seq_t)0; /* allocation failure – sensible default */
        cache[0] = (seq_t)1;         /* a(0) = 1 */
        max_computed = 0;
        sum45 = cache[0];            /* window for n = 1                     */
    }

    /* Grow the cache if the requested index is beyond the current capacity. */
    if ((size_t)(n + 1) > capacity) {
        size_t new_cap = capacity * 2;
        if (new_cap < capacity) new_cap = n + 1;     /* overflow protection */
        if (new_cap < (size_t)(n + 1)) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failure – sensible default */
        cache = tmp;
        capacity = new_cap;
    }

    /* If the value is already computed, return it immediately. */
    if (n <= max_computed)
        return cache[(size_t)n];

    /* Compute subsequent terms up to n. */
    for (idx_t j = max_computed + 1; j <= n; ++j) {
        /* Core recurrence. */
        seq_t term = ((seq_t)46) * sum45;          /* 46 * sum of last up to 45 terms */

        if (j >= 46)                                 /* subtract 1081*a(j-46) */
            term -= ((seq_t)1081) * cache[(size_t)(j - 46)];

        /* Add the inhomogeneous part p(j). */
        if (j <= 45)          term += (seq_t)2;
        else if (j == 46)     term += (seq_t)1;   /* p(46) = 1 */
        /* For j>46, no additional term. */

        cache[(size_t)j] = term;

        /* Update the sliding window sum for the next index. */
        sum45 += cache[(size_t)j];                 /* new term enters the window */
        if (j >= 45)                               /* element leaving the window */
            sum45 -= cache[(size_t)(j - 45)];
    }

    max_computed = n;
    return cache[(size_t)n];
}
