/*
 * A134170  a(n) = the smallest positive integer which, expressed in the form
 *          d*q + r, satisfies q >= r for every d from 1 to n. In other words,
 *          when a(n) is divided by the numbers from 1 to n, the remainder is
 *          never more than the quotient.
 * https://oeis.org/A134170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   satisfies_all
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A134170:
   a(n) = the smallest positive integer m such that for every d in [1,n]
          m = d*q + r with 0 ≤ r < d and r ≤ q.  */

/* Required for bool and dynamic allocation */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/satisfies_all.h"

/* Static cache for already computed values.
   cache[0] is unused (set to 0) to keep indices 1‑based. */
static seq_t *cache = NULL;      /* dynamic array of seq_t values   */
static idx_t cache_max_n = 0;    /* highest index that is already computed */

/* Public entry point.  Computes a(n) with memoisation. */
seq_t a(idx_t n)
{
    /* Grow cache if necessary */
    if (n > cache_max_n) {
        /* Allocate or expand storage */
        idx_t new_size = n + 1;               /* indices 0 … n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {                    /* allocation failure */
            /* In a real application you might handle this differently */
            abort();
        }
        cache = tmp;
        /* Initialise new entries to zero (not strictly needed) */
        for (idx_t i = cache_max_n + 1; i <= n; ++i) cache[i] = (seq_t)0;

        /* Compute new terms using the previous term as a lower bound. */
        for (idx_t k = cache_max_n + 1; k <= n; ++k) {
            seq_t m;
            if (k > 1) {
                m = cache[k - 1];              /* monotonicity gives lower bound */
            } else {
                m = (seq_t)1;
            }
            if (m < (seq_t)k) m = (seq_t)k;    /* must be at least n itself   */
            while (!satisfies_all(k, m)) {
                ++m;
            }
            cache[k] = m;
        }
        cache_max_n = n;
    }
    return cache[n];
}
