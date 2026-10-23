/*
 * A156351  a(n) = Sum_{k=1..n} (-1)^K(k+1)*(K(k+1)-K(k)) where K(k) =
 *          A000002(k).
 * https://oeis.org/A156351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(a(a(n-1)), 2) + a(n-2) (conjectured)
 * Range:     u8 n <= 382, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1) = 1, a(2) = 1 */

#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;   /* stores a(1) .. a(cache_n) at indices 0 .. cache_n-1 */
static idx_t  cache_n = 0;    /* current number of cached terms */

/* Compute the n-th term of OEIS A156351.
   Calls may be in any order; the result is memoized. */
seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; n=0 is outside the domain – return something harmless */
        return (seq_t)0;

    /* Expand the cache if necessary */
    if (n > cache_n) {
        /* Allocate or reallocate space for n terms */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (new_cache == NULL)
            return (seq_t)0; /* unlikely in test harness */
        cache = new_cache;

        /* Initialise the first two terms if they are not yet set */
        if (cache_n < 1) {
            cache[0] = (seq_t)1;          /* a(1) */
        }
        if (cache_n < 2 && n >= 2) {
            cache[1] = (seq_t)1;          /* a(2) */
        }

        /* Compute subsequent terms in ascending order */
        /* Start from the first index for which the recurrence applies (i = 3) */
        idx_t start = cache_n + 1;
        if (start < 3) start = 3;

        for (idx_t i = start; i <= n; ++i) {
            /* i is the 1‑based index of the term to compute */
            idx_t i_minus_1 = i - 2;     /* index of a(i-1) in cache */
            idx_t i_minus_2 = i - 3;     /* index of a(i-2) in cache */

            seq_t a_prev = cache[i_minus_1];          /* a(i-1) */
            /* a_prev is a sequence value, but it also serves as an index. */
            idx_t idx_of_a_prev = (idx_t)a_prev;      /* convert to index (assumed small enough) */
            seq_t inner = cache[idx_of_a_prev - 1];    /* a(a(i-1)) */
            seq_t g      = gcd(inner, (seq_t)2);      /* gcd(a(a(i-1)), 2) */
            seq_t a_i_2  = cache[i_minus_2];          /* a(i-2) */

            cache[i - 1] = g + a_i_2;                 /* store a(i) */
        }

        cache_n = n;  /* update size of the cached array */
    }

    return cache[n - 1];  /* a(n) is stored at index n-1 */
}
