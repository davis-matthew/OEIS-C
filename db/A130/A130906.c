/*
 * A130906  Expansion of e.g.f. exp(x^3/3!)/(1-x).
 * https://oeis.org/A130906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/3⌋} 1/(6^k * k!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the A130906 sequence.  The function remembers previously
   computed values so that calls in any order stay O(n) overall. */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build environment.
   We assume seq_t supports + - * / and the usual numeric
   conversions. */

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;     /* array of a[0..cache_cap-1] */
    static idx_t cache_cap = 0;     /* current size of cache */
    static seq_t fact   = (seq_t)1; /* (cache_cap-1)! at start */
    static seq_t Ex     = (seq_t)1; /* extra term for k==3*m */

    /* initialise cache to hold a[0] */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1; /* a(0) = 1   */
    }

    /* extend cache up to requested n (if necessary) */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            exit(EXIT_FAILURE);
        }
        cache = tmp;

        for (idx_t k = cache_cap; k <= n; ++k) {
            /* Update factorial: fact = k!  (factorial of current k) */
            fact *= (seq_t)k;

            /* Update extra term when k is a multiple of 3:
               Ex = (3m)! / (6^m * m!)  for m = k/3  */
            if ((k % 3) == 0) {
                idx_t m = k / 3;
                seq_t num1 = (seq_t)(3 * m - 2);
                seq_t num2 = (seq_t)(3 * m - 1);
                seq_t num3 = (seq_t)(3 * m);
                seq_t denom = (seq_t)6 * (seq_t)m;
                seq_t numerator = Ex * num1 * num2 * num3;
                Ex = numerator / denom;
            }

            /* Compute a(k) using the recurrence */
            seq_t val = cache[k - 1] * (seq_t)k; /* k * a(k-1) */
            if ((k % 3) == 0) {
                val += Ex;
            }
            cache[k] = val;
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
