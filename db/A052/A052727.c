/*
 * A052727  Expansion of e.g.f. 1/2-1/2*(1-4*x-4*x^2)^(1/2).
 * https://oeis.org/A052727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * A025227(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=4,
 *   for n ≥ 1:  a(n+2) = (4*n² – 4) * a(n) + (4*n + 2) * a(n+1) */

#include <stdlib.h>

/* Compute A052727(a) for index n */
seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache   = NULL;   /* array of computed terms */
    static idx_t cache_len = 0;     /* number of elements stored (cache_len = size) */

    /* Initialise cache with first three non‑zero terms. */
    if (cache_len == 0) {
        cache_len = 3;                     /* indices 0, 1, 2 will be stored */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;       /* allocation failure – return 0 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
    }

    /* Return already computed value if available. */
    if (n < cache_len)
        return cache[n];

    /* Ensure the cache is large enough. */
    if (n + 1 > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed; keep the old cache and return a safe value. */
            return (seq_t)0;
        }
        cache = tmp;
        cache_len = n + 1;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = (idx_t)(cache_len - 1); i <= n; ++i) {
        if (i < 3)   /* already initialized: a(0)=0, a(1)=1, a(2)=4 */
            continue;

        /* n in the recurrence is i-2 */
        idx_t k = i - 2;

        /* (4*k*k - 4) * a(k) + (4*k + 2) * a(k+1) */
        seq_t term1 = ((seq_t)4 * k * k - (seq_t)4) * cache[k];
        seq_t term2 = ((seq_t)4 * k + (seq_t)2) * cache[k + 1];
        cache[i] = term1 + term2;
    }

    return cache[n];
}
