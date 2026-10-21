/*
 * A074358  Coefficient of q^1 in nu(n), where nu(0)=1, nu(1)=b and, for n >=
 *          2, nu(n) = b*nu(n-1) + lambda*(1 + q + q^2 + ... +
 *          q^(n-2))*nu(n-2) with (b,lambda)=(2,2).
 * https://oeis.org/A074358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   c(0)=c(1)=c(2)=0; d(0)=1, d(1)=2, d(n)=2*d(n-1)+2*d(n-2);
 *            c(n)=2*c(n-1)+2*(c(n-2)+d(n-2)) for n≥3. This c(n) is the
 *            coefficient of q¹ in ν(n)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static caches for coefficients */
    static seq_t *c_cache = NULL;
    static seq_t *d_cache = NULL;
    static idx_t cap   = 0;   /* allocation size */
    static idx_t size  = 0;   /* number of values already computed */

    /* initialise on first call */
    if (cap == 0) {
        cap  = 8;            /* initial capacity */
        size = 2;            /* we hold values 0 and 1 initially */
        c_cache = (seq_t*)malloc(cap * sizeof(seq_t));
        d_cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!c_cache || !d_cache)
            return 0;       /* fallback if allocation fails */

        /* base values */
        c_cache[0] = 0;
        c_cache[1] = 0;
        d_cache[0] = 1;
        d_cache[1] = 2;
    }

    /* enlarge cache if necessary */
    if (n + 1 > cap) {
        idx_t newcap = cap;
        while (newcap <= n)
            newcap <<= 1;          /* double until sufficient */
        seq_t *tmpc = (seq_t*)realloc(c_cache, newcap * sizeof(seq_t));
        seq_t *tmpd = (seq_t*)realloc(d_cache, newcap * sizeof(seq_t));
        if (!tmpc || !tmpd)
            return c_cache[size-1];     /* fallback */
        c_cache = tmpc;
        d_cache = tmpd;
        cap = newcap;
    }

    /* compute values up to n */
    for (idx_t i = size; i <= n; ++i) {
        if (i == 0) {
            c_cache[0] = 0;
            d_cache[0] = 1;
        } else if (i == 1) {
            c_cache[1] = 0;
            d_cache[1] = 2;
        } else if (i == 2) {
            c_cache[2] = 0;
            d_cache[2] = ((seq_t)2) * d_cache[1] + ((seq_t)2) * d_cache[0];
        } else {
            /* recurrence for n ≥ 3 */
            seq_t c_prev  = c_cache[i-1];
            seq_t c_prev2 = c_cache[i-2];
            seq_t d_prev2 = d_cache[i-2];
            seq_t d_prev  = d_cache[i-1];

            c_cache[i] = ((seq_t)2) * c_prev + ((seq_t)2) * (c_prev2 + d_prev2);
            d_cache[i] = ((seq_t)2) * d_prev  + ((seq_t)2) * d_prev2;
        }
    }
    size = n + 1;      /* update number of computed values */

    return c_cache[n];
}
