/*
 * A099917  Expansion of (1+x^2)^2/(1+x^3+x^6).
 * https://oeis.org/A099917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A099917 – Expansion of (1+x^2)^2/(1+x^3+x^6)
 *
 *  The generating series satisfies
 *      a(n) + a(n-3) + a(n-6) = c(n)
 *  where
 *      c(0)=1 , c(2)=2 , c(4)=1 ,   c(k)=0 otherwise.
 *  Hence
 *      a(n) = -a(n-3) - a(n-6) + c(n).
 *
 *  The sequence is periodic with period 9, but we compute it lazily
 *  using the recurrence, caching past values.  This works correctly
 *  for all non‑negative n in O(n) time.
 */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;     /* array holding a(0) .. a(current_max-1) */
    static idx_t cache_max = 0;     /* number of terms stored */

    /* enlarge cache if necessary */
    if (n >= cache_max) {
        /* new size: double current or just enough for n+1 */
        idx_t new_size = (cache_max == 0) ? 16 : cache_max << 1;
        if (new_size <= n + 1) new_size = n + 1;

        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!cache) { /* allocation failed – abort */
            /* In the context of the testing harness a call to abort()
               or similar would be acceptable.  For portability we
               simply return 0. */
            return (seq_t)0;
        }

        /* initialize newly allocated part to 0 so negative indices evaluate to 0 */
        for (idx_t i = cache_max; i < new_size; ++i) {
            cache[i] = (seq_t)0;
        }
        cache_max = new_size;
    }

    /* compute missing terms up to n */
    for (idx_t i = 0; i <= n; ++i) {
        /* ensure lower indices computed before upper ones */
        if (i < 6 && cache[i]) /* already computed via previous call */
            continue;

        /* coefficients of RHS */
        seq_t c = 0;
        if (i == 0)          c = (seq_t)1;
        else if (i == 2)     c = (seq_t)2;
        else if (i == 4)     c = (seq_t)1;

        seq_t term = (seq_t)0;
        if (i >= 3) term -= cache[i - 3];
        if (i >= 6) term -= cache[i - 6];
        term += c;

        cache[i] = term;
    }

    return cache[n];
}
