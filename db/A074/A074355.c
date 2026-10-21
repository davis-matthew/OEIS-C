/*
 * A074355  Coefficient of q^1 in nu(n), where nu(0)=1, nu(1)=b and, for n>=2,
 *          nu(n)=b*nu(n-1)+lambda*(1+q+q^2+...+q^(n-2))*nu(n-2) with
 *          (b,lambda)=(1,3).
 * https://oeis.org/A074355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^1 in nu(n) where nu(0)=1, nu(1)=1,
 *            nu(n)=nu(n-1)+3*(1+q+…+q^(n-2))*nu(n-2) (n≥2). Initial values:
 *            a(0)=0, a(1)=0. Recurrence (derived from the product term):
 *            a(n)=a(n-1)+3*(a(n-2)+d0(n-2)) for n≥3, a(2)=0 (satisfies the
 *            same if d0(0) is omitted). Here d0(n) is the constant...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised tables for a(n) and constant terms d0(n) */
static seq_t *a_cache = NULL;
static seq_t *d0_cache = NULL;
static idx_t max_cached = 0;

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Handle trivial indices directly (no caching needed) */
    if (n == 0 || n == 1)
        return (seq_t)0;

    /* Initialise memoisation storage on first use */
    if (max_cached == 0)
    {
        /* Allocate space for indices 0 and 1 */
        size_t init_len = 2;
        a_cache  = (seq_t*)calloc(init_len, sizeof(seq_t));
        d0_cache = (seq_t*)calloc(init_len, sizeof(seq_t));
        if (!a_cache || !d0_cache)
            return (seq_t)0;                     /* memory failure → 0 */

        /* Base values */
        a_cache[0] = a_cache[1] = (seq_t)0;
        d0_cache[0] = (seq_t)1;
        d0_cache[1] = (seq_t)1;
        max_cached = 1;
    }

    /* Grow the cached data if necessary */
    if (n > max_cached)
    {
        size_t new_len = (size_t)(n + 1);
        a_cache  = (seq_t*)realloc(a_cache,  new_len * sizeof(seq_t));
        d0_cache = (seq_t*)realloc(d0_cache, new_len * sizeof(seq_t));
        if (!a_cache || !d0_cache)
            return (seq_t)0;                     /* memory failure → 0 */

        /* Compute new entries sequentially */
        for (idx_t i = max_cached + 1; i <= n; ++i)
        {
            /* d0[i] = d0[i-1] + 3*d0[i-2] */
            d0_cache[i] = d0_cache[i-1] + (seq_t)3 * d0_cache[i-2];

            /* S = a[i-2] + (i-2>=1 ? d0[i-2] : 0) */
            seq_t S = a_cache[i-2];
            if (i - 2 >= 1)
                S += d0_cache[i-2];

            /* a[i] = a[i-1] + 3*S */
            a_cache[i] = a_cache[i-1] + (seq_t)3 * S;
        }

        max_cached = n;
    }

    return a_cache[n];
}
