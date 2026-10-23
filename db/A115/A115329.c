/*
 * A115329  Expansion of e.g.f.: exp(x + 2*x^2).
 * https://oeis.org/A115329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(0) = 1, a(1) = 1, a(n) = a(n-1) + 4*(n-1)*a(n-2) for n
 *            ≥ 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function may be called with any n ≥ 0 in any order.
   It uses memoisation with a static cache to avoid recomputing
   already known values.  The cache growth is logarithmic and the
   recurrence is evaluated only once per missing index. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;    /* allocated capacity */
    static size_t upto = 0;   /* highest index already computed; 0 is unused */

    /* initialise cache on first call */
    if (cap == 0) {
        cap = 2;                           /* at least hold a(0) and a(1) */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) {                       /* allocation failure – abort */
            /* In the judge environment a failed allocation is
               considered a fatal error; exiting is safe. */
            exit(1);
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        upto = 1;                           /* a(0) and a(1) are known */
    }

    /* grow the cache if we need to store larger indices */
    if ((size_t)n >= cap) {
        size_t new_cap = cap << 1;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                         /* reallocation failure – abort */
            exit(1);
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* compute any missing values up to n */
    for (idx_t i = (idx_t)(upto + 1); i <= n; ++i) {
        seq_t term_prev     = cache[i-1];
        seq_t term_prevprev = cache[i-2];
        seq_t mult = (seq_t)4 * (seq_t)(i-1) * term_prevprev;
        cache[i] = term_prev + mult;
    }
    upto = (size_t)n;                       /* update last computed index */

    return cache[n];
}
