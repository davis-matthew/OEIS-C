/*
 * A063414  Number of digits in prime(n)! (A039716).
 * https://oeis.org/A063414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor( Σ_{k=2}^{prime(n)} log10(k) ) + 1
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "../utils/_prime_limit_for_idx.h"   /* _prime_limit_for_idx() */
#include "../utils/build_primes_upto.h"      /* build_primes_upto() */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1 – unused */

    /* Prime list ---------------------------------------------------------- */
    static uint64_t *pr = NULL;  /* array of primes (pr[0] = 2) */
    static size_t pcnt = 0;      /* number of primes stored   */

    if ((size_t)n > pcnt) {      /* need more primes            */
        idx_t lim = _prime_limit_for_idx(n);  /* upper bound on nth prime */
        free(pr);
        build_primes_upto(lim, &pr, &pcnt);   /* fills pr[0..pcnt-1] */
    }
    uint64_t p = pr[n - 1];     /* the n-th prime                */

    /* Cumulative log10 values -------------------------------------------- */
    static long double *logcum = NULL;  /* logcum[i] = Σ_{k=1}^{i} log10(k) */
    static size_t lcap = 0;              /* allocated size of logcum       */

    if (lcap <= (size_t)p) {              /* need to extend array         */
        size_t old = lcap;
        size_t newcap = (size_t)p + 1;
        long double *tmp = (long double *)realloc(logcum, newcap * sizeof(long double));
        logcum = tmp;
        if (old == 0) {                    /* initialise first element    */
            logcum[0] = 0.0L;
            old = 1;
        }
        for (size_t i = old; i <= (size_t)p; ++i) {
            logcum[i] = logcum[i - 1] + log10l((long double)i);
        }
        lcap = newcap;
    }

    /* Digit count -------------------------------------------------------- */
    long double val = logcum[(size_t)p];
    uint64_t digits64 = (uint64_t)(floorl(val + 1e-12L) + 1.0L); /* epsilon to avoid rounding errors */

    return (seq_t)digits64;
}
