/*
 * A081557  Binomial transform of expansion of exp(cosh(x)), A005046.
 * https://oeis.org/A081557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; for n ≥ 0, a(n+1) = a(n) + Σ_{k odd, 1≤k≤n} C(n,k) ·
 *            a(n-k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where C(n,k) is the binomial coefficient.  This follows from the e.g.f. 
    A(x) = exp(cosh(x)+x-1).  The code memoises results for repeated calls. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        /* grow cache to hold indices up to n */
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)  /* allocation failure – abort */
            abort();
        cache = new_cache;

        /* initialise if this is the first call */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;        /* a(0) = 1 */
            cache_size = 1;
        }

        /* compute the missing terms up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            idx_t prev = i - 1;                 /* n in the recurrence */
            seq_t sum = cache[prev];             /* term for k = 0 */
            seq_t bin = (seq_t)1;                /* C(prev,0) */

            for (idx_t k = 1; k <= prev; ++k) {
                /* update binomial coefficient C(prev,k) */
                bin = bin * ((prev) - (k - 1));
                bin = bin / k;

                if (k & 1) {                     /* odd k only add */
                    sum += bin * cache[prev - k];
                }
            }
            cache[i] = sum;
        }
        cache_size = new_size;
    }

    return cache[n];
}
