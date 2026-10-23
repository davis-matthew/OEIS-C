/*
 * A117497  Length of shortest sequence b with b(0) = 1, b(i+1) = b(i)+d where
 *          d|b(i) and b(k) = n.
 * https://oeis.org/A117497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(n)=1+min_{d|n, d<n} a(n-d) (plus the trivial
 *            recurrence for powers of two: a(2^k)=k). This implementation
 *            uses dynamic programming and a minimal cache that grows on
 *            demand
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>   /* for bool, true, false */

/* The sequence is defined for n >= 1.  For n < 1 the function returns 0. */
seq_t a(idx_t n)
{
    /* Cache of computed values, 1‑based indexing.  */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* number of slots allocated in cache */
    static idx_t  max_computed = 0; /* highest n that has been filled */

    /* Handle out-of-bounds request (offset is 1).  */
    if (n < 1) return (seq_t)0;

    /* Allocate or grow the cache to accommodate index n.  */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);        /* allocation failed */
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Initialisation of base case a(1)=0 performed only once.  */
    if (max_computed == 0) {
        cache[1] = (seq_t)0;
        max_computed = 1;
    }

    /* Compute terms up to n if they are not yet known.  */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t best = (seq_t)0;
        bool first = true;

        /* Enumerate all proper divisors d of i (1 <= d < i).  */
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d != 0) continue;           /* not a divisor */

            /* d is a divisor.  */
            if (d < i) {
                seq_t cand = cache[i - d] + (seq_t)1;
                if (first || cand < best) {
                    best = cand;
                    first = false;
                }
            }

            /* Complementary divisor i/d, skip if equal to d.  */
            idx_t d2 = i / d;
            if (d2 != d && d2 < i) {
                seq_t cand = cache[i - d2] + (seq_t)1;
                if (first || cand < best) {
                    best = cand;
                    first = false;
                }
            }
        }

        /* For i>1 the loop above always finds at least the divisor 1.  */
        cache[i] = best;
    }

    max_computed = n;
    return cache[n];
}
