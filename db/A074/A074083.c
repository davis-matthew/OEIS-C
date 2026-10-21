/*
 * A074083  Coefficient of q^3 in nu(n), where nu(0)=1, nu(1)=b and, for n>=2,
 *          nu(n)=b*nu(n-1)+lambda*(1+q+q^2+...+q^(n-2))*nu(n-2) with
 *          (b,lambda)=(1,1).
 * https://oeis.org/A074083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A074083:  a(n) = coefficient of q^3 in nu(n)
    where nu(0)=1, nu(1)=1 and for n>=2
        nu(n) = nu(n-1) + (1+q+q^2+...+q^(n-2)) * nu(n-2)
    Only the coefficients up to q^3 are needed; the recurrence is
    evaluated iteratively, keeping the last two polynomials truncated to
    degree 3.  The result for a(n) is the coefficient of q^3.
    This method uses only addition and works for any unsigned seq_t
    (including 8/16/32/64/128 bit and arbitrary‑precision builds).  */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  Static storage for memoisation and for the two latest polynomials
        `prev2` (nu(n-2)) and `prev1` (nu(n-1)) truncated to degree 3.  */
    static seq_t *cache = NULL;          /* cache of a(0)...a(k)   */
    static size_t cache_size = 0;        /* number of stored values */
    static size_t cache_cap   = 0;        /* allocated capacity of cache */
    static seq_t prev1[4];
    static seq_t prev2[4];
    static int    init    = 0;            /* flag for first call  */

    if (!init) {
        /* initialise cache with a(0)=0, a(1)=0 */
        cache_cap = 4;
        cache    = (seq_t*)malloc(sizeof(seq_t)*cache_cap);
        cache[0] = 0;  /* a(0) */
        cache[1] = 0;  /* a(1) */
        cache_size = 2;

        /* initialise polynomials nu(0) and nu(1) */
        prev1[0] = 1;  prev1[1] = 0;  prev1[2] = 0;  prev1[3] = 0;
        prev2[0] = 1;  prev2[1] = 0;  prev2[2] = 0;  prev2[3] = 0;
        init = 1;
    }

    /*  Ensure cache covers the requested index.  */
    if (cache_size <= n) {
        /* grow cache array if necessary  */
        if (cache_cap <= n) {
            while (cache_cap <= n) cache_cap = (cache_cap ? cache_cap * 2 : 4);
            cache = (seq_t*)realloc(cache, sizeof(seq_t)*cache_cap);
        }

        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t curr[4] = {0,0,0,0};

            /* add nu(i-1) */
            for (int d = 0; d < 4; ++d) curr[d] += prev1[d];

            /* add contributions from nu(i-2) multiplied by (1+q+...+q^(i-2)) */
            if (i >= 2) {
                idx_t max_k = 3;
                if (max_k > (i-2)) max_k = i-2;
                for (int k = 0; k <= max_k; ++k) {
                    for (int j = 0; j < 4; ++j) {
                        if (j + k <= 3)
                            curr[j + k] += prev2[j];
                    }
                }
            }

            /* store coefficient of q^3 */
            cache[cache_size++] = curr[3];

            /* shift polynomials for next iteration   */
            seq_t temp[4];
            for (int d = 0; d < 4; ++d) temp[d] = prev1[d];
            for (int d = 0; d < 4; ++d) prev1[d] = curr[d];
            for (int d = 0; d < 4; ++d) prev2[d] = temp[d];
        }
    }

    return cache[n];
}
