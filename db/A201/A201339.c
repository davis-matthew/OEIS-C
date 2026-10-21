/*
 * A201339  Expansion of e.g.f. exp(x) / (3 - 2*exp(x)).
 * https://oeis.org/A201339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 1 + 2 * Sum_{k=0}^{n-1} binomial(n,k) * a(k)
 *            for n >= 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple O(n²) recurrence with memoisation.
   It works for built‑in integer types as well as when seq_t is an
   arbitrary‑precision integer class because all arithmetic is performed
   on seq_t objects. */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Cache to store already computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* current allocated size */

    /* initialise cache for a(0) */
    if (cap == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;      /* allocation failure, bail out */
        cache[0] = (seq_t)1;       /* a(0) = 1 */
        cap = 1;
    }

    /* Grow cache if necessary */
    if (n + 1 > cap) {
        size_t old_cap = cap;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!cache) return 0;          /* allocation failure */
        cap = (size_t)(n + 1);

        /* Compute new entries from old_cap up to n */
        for (idx_t m = old_cap; m <= n; ++m) {
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;   /* binom(m,0) */

            for (idx_t k = 0; k < m; ++k) {
                sum += cache[k] * binom;

                /* Update binom to binom(m,k+1) */
                binom *= (seq_t)(m - k);
                binom /= (seq_t)(k + 1);
            }

            cache[m] = (seq_t)1 + (seq_t)2 * sum;
        }
    }

    return cache[n];
}
