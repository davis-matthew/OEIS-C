/*
 * A063090  a(n)/(n*n!) is the average number of comparisons needed to find a
 *          node in a binary search tree containing n nodes inserted in a
 *          random order.
 * https://oeis.org/A063090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (2*n - 1)*(n-1)! + (n + 1)*a(n-1), with a(1) = 1. This
 *            recurrence is O(n) and memoized
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for i >= 1, index 0 unused. */
    static seq_t *cache = NULL;      /* array of seq_t values */
    static idx_t   cap   = 0;        /* allocated capacity (index 0 .. cap-1) */
    static idx_t   max_n = 0;        /* maximum n for which a(n) has been computed */
    static seq_t   fact  = 1;        /* factorial of max_n (i.e. max_n!) */

    /* guard against calls with n == 0 (not part of the sequence) */
    if (n == 0)
        return 0;

    /* Initialisation on first call */
    if (cache == NULL) {
        /* Start with capacity for indices 0..1 (i.e. size 2). */
        cap = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) return 0;      /* out‑of‑memory: best‑effort fallback */
        cache[0] = 0;              /* unused */
        cache[1] = (seq_t)1;       /* a(1) = 1 */
        max_n = 1;
        fact  = (seq_t)1;          /* 1! = 1 */
    }

    /* If we already have a(n), return it immediately. */
    if (n <= max_n)
        return cache[n];

    /* Ensure the array can hold indices up to n. */
    if (n + 1 > cap) {
        idx_t new_cap = cap ? cap * 2 : 8;
        while (new_cap <= n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) {
            /* realloc failed – return a reasonable value (fallback) */
            return 0;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Compute a(i) and factorials for i = max_n+1 .. n. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* Update factorial: fact becomes i! */
        fact = fact * (seq_t)i;

        /* (i-1)! is fact / i (exact division). */
        seq_t fact_prev = fact / (seq_t)i;

        /* term1 = (2*i - 1) * (i-1)! */
        seq_t term1 = ((seq_t)2 * (seq_t)i - 1) * fact_prev;

        /* term2 = (i + 1) * a(i-1) */
        seq_t term2 = ((seq_t)i + 1) * cache[i - 1];

        cache[i] = term1 + term2;
    }

    max_n = n;
    return cache[n];
}
