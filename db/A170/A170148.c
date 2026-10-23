/*
 * A170148  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n)=p(n)+41*Σ_{k=1}^{37}a(n-k)–861*a(n-38)
 *            where p(n)=1 if n=0, p(n)=2 if 1≤n≤37, p(n)=1 if n=38, else
 *            p(n)=0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External integer types supplied by the build environment.
   They are already defined in the surrounding build framework. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms, indexed by seq_t index as size_t. */
    static seq_t *cache      = NULL;
    static size_t  capacity  = 0;      /* allocated size of cache */
    static idx_t   next_free = 0;      /* index of first element not yet filled */

    /* Allocate or grow cache to accommodate 0..n. */
    if ((size_t)(n) + 1 > capacity) {
        size_t new_cap = capacity ? capacity : 1;
        while (new_cap < (size_t)(n) + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure: abort silently */
            return (seq_t)0;
        }
        cache     = tmp;
        capacity  = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = next_free; i <= n; ++i) {
        seq_t sum = 0;

        /* 41 * sum of the previous 37 terms */
        for (idx_t k = 1; k <= 37; ++k) {
            if (i >= k) {
                sum += (seq_t)41 * cache[(size_t)(i - k)];
            }
        }

        /* subtract 861 * a(i-38) if i >= 38 */
        if (i >= 38) {
            sum -= (seq_t)861 * cache[(size_t)(i - 38)];
        }

        /* add the constant term p(i) */
        if (i == 0) {
            sum += (seq_t)1;          /* p(0) = 1 */
        } else if (i <= 37) {
            sum += (seq_t)2;          /* p(1..37) = 2 */
        } else if (i == 38) {
            sum += (seq_t)1;          /* p(38) = 1 */
        }
        /* otherwise p(i) = 0, nothing to add */

        cache[(size_t)i] = sum;
    }
    next_free = n + 1;
    return cache[(size_t)n];
}
