/*
 * A032952  Expansion of (1+x*C^4)*C^2, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A032952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (n+5)*(19*n-8)*a(n) = (113*n^2+307*n-76)*a(n-1) -
 *            2*(2*n-1)*(37*n+36)*a(n-2); initial values: a(0)=1, a(1)=3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* cache of computed values          */
static size_t cache_cap = 0;     /* allocated capacity in elements     */
static idx_t  last_computed = 0; /* highest index already cached       */

seq_t a(idx_t n)
{
    /* Handle the base cases directly.  */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Ensure the cache can hold indices 0..n.  */
    idx_t needed = n + 1;
    if (cache_cap < (size_t)needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = needed;

        /* Initialize the first two entries if this is the first allocation. */
        if (last_computed < 1) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)3;
            last_computed = 1;
        }
    }

    /* Compute any missing values up to n. */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        seq_t a1 = cache[k-1];   /* a(k-1) */
        seq_t a2 = cache[k-2];   /* a(k-2) */

#ifndef __cplusplus
        /* C implementation: use unsigned __int128 for intermediate arithmetic. */
        unsigned __int128 sn     = (unsigned __int128)k;
        unsigned __int128 coeff1 = (unsigned __int128)113 * sn * sn
                                 + (unsigned __int128)307 * sn
                                 - (unsigned __int128)76;
        unsigned __int128 coeff2 = (unsigned __int128)2 * (sn * 2 - (unsigned __int128)1)
                                 * (sn * 37 + (unsigned __int128)36);
        unsigned __int128 denom  = (sn + (unsigned __int128)5)
                                 * (sn * 19 - (unsigned __int128)8);

        unsigned __int128 numer =
            coeff1 * (unsigned __int128)a1 - coeff2 * (unsigned __int128)a2;

        cache[k] = (seq_t)(numer / denom);
#else
        /* C++ implementation: use seq_t arithmetic directly. */
        seq_t sn = (seq_t)k;
        seq_t coeff1 = (seq_t)113 * sn * sn
                     + (seq_t)307 * sn
                     - (seq_t)76;
        seq_t coeff2 = (seq_t)2 * ((sn * (seq_t)2) - (seq_t)1)
                     * ((sn * (seq_t)37) + (seq_t)36);
        seq_t denom  = (sn + (seq_t)5) * ((sn * (seq_t)19) - (seq_t)8);

        seq_t numer = coeff1 * a1 - coeff2 * a2;
        cache[k] = numer / denom;
#endif
    }

    last_computed = n;
    return cache[n];
}
