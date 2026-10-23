/*
 * A158197  Expansion of (1-x^2*c(x)^4)/(1-4*x*c(x)^2), c(x) the g.f. of
 *          A000108.
 * https://oeis.org/A158197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expansion of (1‑x^2*c(x)^4)/(1‑4*x*c(x)^2) where c(x) is the Catalan generating function.
   Recurrence (valid for n ≥ 3):
      4*(n+1)*a(n) + (-81*n+23)*a(n-1) + 10*(51*n-70)*a(n-2) + 500*(-2*n+5)*a(n-3) = 0
   Initial values:
      a(0) = 1, a(1) = 4, a(2) = 23
   The function below uses simple memoisation; it works for any build
   (integral or arbitrary‑precision seq_t).  */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache = NULL;
    static size_t cached = 0;   /* number of elements currently stored */

    /* If the value is already in cache, return it immediately. */
    if (n < cached)
        return cache[n];

    /* Grow the cache to hold indices up to n. */
    size_t need = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
    if (!new_cache)
        return 0;        /* allocation failure – unlikely for the tested limits */

    /* Construct the newly allocated seq_t objects when compiling under C++. */
    if (new_cache != cache) {
#ifdef __cplusplus
        for (size_t i = cached; i < need; ++i)
            new(&new_cache[i]) seq_t();   /* default‑construct */
#endif
    }

    /* Compute terms that the caller may have asked for. */
    for (idx_t i = (idx_t)cached; i <= n; ++i)
    {
        if (i == 0)
            new_cache[0] = 1;
        else if (i == 1)
            new_cache[1] = 4;
        else if (i == 2)
            new_cache[2] = 23;
        else
        {
            /* Coefficients for the recurrence.  Use 64‑bit signed integers
               to avoid overflow for n ≤ 10000. */
            int64_t c1 = -81 * (int64_t)i + 23;          /* coefficient of a(n-1) */
            int64_t c2 = 10 * ((int64_t)51 * i - 70);    /* coefficient of a(n-2) */
            int64_t c3 = 500 * (-2 * (int64_t)i + 5);    /* coefficient of a(n-3) */

            /* Separate positive and negative contributions to keep the
               intermediate value non‑negative (seq_t is unsigned). */
            seq_t sum_pos = 0, sum_neg = 0;

            if (c1 >= 0)
                sum_pos += (seq_t)c1 * new_cache[i - 1];
            else
                sum_neg += (seq_t)(-c1) * new_cache[i - 1];

            if (c2 >= 0)
                sum_pos += (seq_t)c2 * new_cache[i - 2];
            else
                sum_neg += (seq_t)(-c2) * new_cache[i - 2];

            if (c3 >= 0)
                sum_pos += (seq_t)c3 * new_cache[i - 3];
            else
                sum_neg += (seq_t)(-c3) * new_cache[i - 3];

            /* Numerator of the recurrence (t_neg ≥ t_pos). */
            seq_t numerator = sum_neg - sum_pos;
            seq_t denom     = 4 * ((seq_t)i + 1);
            new_cache[i] = numerator / denom;
        }
    }

    /* Update cache pointer and size.  The old memory is automatically
       reallocated by realloc; no need to free it separately. */
    cache   = new_cache;
    cached  = need;

    return cache[n];
}
