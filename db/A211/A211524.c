/*
 * A211524  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w=3x+5y.
 * https://oeis.org/A211524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A211524: the number of ordered triples (w,x,y)
   with w=3x+5y and w,x,y∈{1,…,n}.  The value a(n) equals
   the number of pairs (x,y) with 3x+5y ≤ n.  Computed by
   direct summation:  a(n)=∑_{y=1}^{⌊(n-3)/5⌋} ⌊(n-5y)/3⌋ for n≥8,
   otherwise a(n)=0.  Results are memoised for efficiency. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic cache for computed values     */
static idx_t cache_max = 0;   /* number of indices currently cached    */

/* compute a single term without using the cache */
static seq_t compute_term(idx_t n)
{
    if (n < 8) return 0;

    idx_t y_max = (n - 3) / 5;          /* floor((n-3)/5) */
    if (y_max > n) y_max = n;           /* safety, though not needed          */

    seq_t total = 0;

    for (idx_t y = 1; y <= y_max; ++y) {
        /* (n - 5*y) is ≥ 3 by definition of y_max, so integer division is safe */
        seq_t xmax = (seq_t)(n - 5 * y) / 3;
        total += xmax;
    }

    return total;
}

seq_t a(idx_t n)
{
    /* Grow the cache to accommodate index n */
    if (n >= cache_max) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */
        cache = tmp;

        /* Compute and store missing entries */
        for (idx_t i = cache_max; i <= n; ++i) {
            cache[i] = compute_term(i);
        }
        cache_max = new_cap;
    }
    return cache[n];
}
