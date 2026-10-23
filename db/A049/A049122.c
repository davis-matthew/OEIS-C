/*
 * A049122  Revert transform of x*(1 + 2*x)/(1 + 3*x + x^2).
 * https://oeis.org/A049122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   recurrence with initial values a(1)=1, a(2)=1, a(3)=0, 2*n*a(n)
 *            = -3*(n-2)*a(n-1) - (8*n - 27)*a(n-2) + 5*(n-3)*a(n-3)
 * Range:     i8 n <= 9, i16 n <= 16, i32 n <= 30, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the nth term of OEIS A049122.  The function is memoised
   across calls and can be invoked in any order.  It works for any
   signed integer type `seq_t` and any unsigned integer type `idx_t`
   supplied by the build environment. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n < 1 */

    /* static cache to store computed values */
    static seq_t *cache     = NULL;
    static idx_t  cache_sz  = 0;     /* size of allocated array */
    static idx_t  last_known = 0;    /* highest index already computed */

    /* Expand the cache if needed. */
    if (n + 1 > cache_sz)
    {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp && n + 1 > 0)
            return 0;                 /* allocation failure – fall back to 0 */
        cache     = tmp;
        cache_sz  = n + 1;
    }

    /* initialise the initial values if they have not yet been set. */
    if (last_known < 1 && n >= 1) cache[1] = (seq_t)1;
    if (last_known < 2 && n >= 2) cache[2] = (seq_t)1;
    if (last_known < 3 && n >= 3) cache[3] = (seq_t)0;

    /* Compute further terms if necessary. */
    for (idx_t i = last_known + 1; i <= n; ++i)
    {
        if (i <= 3)
        {
            /* already initialised above – nothing to do */
            continue;
        }

        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];

        seq_t t1 = (seq_t)3 * (seq_t)(i - 2) * a1;
        seq_t t2 = (seq_t)(8 * (seq_t)i - 27) * a2;
        seq_t t3 = (seq_t)5 * (seq_t)(i - 3) * a3;

        seq_t rhs   = -t1 - t2 + t3;
        seq_t denom = 2 * (seq_t)i;
        cache[i]    = rhs / denom;
    }

    last_known = (idx_t) (n > last_known ? n : last_known);

    return cache[n];
}
