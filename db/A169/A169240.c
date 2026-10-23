/*
 * A169240  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence from the generating function:
 *   a(n) = 33 * sum_{k=1}^{27} a(n-k) – 561 * a(n-28) + p(n)
 *   where p(0)=1, p(1..27)=2, p(28)=1, and p(n)=0 for n>28.
 * Cached to allow calls in any order. */
seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;   /* allocated capacity in terms of cache */
    static idx_t   cache_size = 0;  /* number of terms already computed */

    /* Ensure enough storage. Default minimum capacity 20000 for speed. */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 20000) new_cap = 20000;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* fall back if allocation fails */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Constants used in the recurrence. */
    const seq_t C33  = (seq_t)33;
    const seq_t C561 = (seq_t)561;

    /* Compute terms that have not yet been computed. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t value;
        if (i == 0) {
            value = 1;
        } else {
            /* Compute the p(i) term from the numerator of G.f. */
            seq_t p = 0;
            if (i == 28)       p = 1;
            else if (i <= 27)  p = 2;
            /* for i > 28, p remains 0 */

            /* Sum of the previous 27 terms multiplied by 33. */
            idx_t kmax = (i < 27) ? i : 27;
            seq_t sum = 0;
            for (idx_t k = 1; k <= kmax; ++k)
                sum += C33 * cache[(size_t)(i - k)];

            /* Subtract 561 * a(i-28) when i >= 28. */
            if (i >= 28)
                sum -= C561 * cache[(size_t)(i - 28)];

            value = sum + p;
        }
        cache[(size_t)i] = value;
    }

    cache_size = n + 1;
    return cache[(size_t)n];
}
