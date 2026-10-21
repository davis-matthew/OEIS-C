/*
 * A078532  Coefficients of power series that satisfies A(x)^3 - 9*x*A(x)^4 =
 *          1, A(0)=1.
 * https://oeis.org/A078532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(2n)*binomial(4n/3-2/3, n)/(n+1) Recurrence:
 *            n*(n-1)*(n+1)*a(n) -216*(4n-5)*(2n-1)*(4n-11)*a(n-3)=0 with
 *            a(0)=1, a(1)=3, a(2)=27
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* Cached values for memoisation */
static seq_t *cache = NULL;   /* array of seq_t, index = n */
static idx_t cache_len = 0;   /* current size of cache (number of indices stored) */

/* Ensure cache has room for indices up to n (inclusive) */
static void ensure_cache_up_to(idx_t n) {
    if (n < cache_len)
        return; /* already large enough */

    idx_t new_len = n + 1; /* need indices 0..n */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    /* initialise new elements to 0 – zero means “not yet computed” */
    for (idx_t i = cache_len; i <= n; ++i)
        tmp[i] = (seq_t)0;
    cache = tmp;
    cache_len = new_len;
}

/* Public sequence function */
seq_t a(idx_t n) {
    /* expand cache if necessary */
    ensure_cache_up_to(n);

    /* return cached value if available */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* compute base values */
    if (n == 0) {
        cache[0] = (seq_t)1;
        return cache[0];
    }
    if (n == 1) {
        cache[1] = (seq_t)3;
        return cache[1];
    }
    if (n == 2) {
        cache[2] = (seq_t)27;
        return cache[2];
    }

    /* recurrence for n >= 3:
       a[n] = [216*(4n-5)*(2n-1)*(4n-11)*a[n-3]] / [n*(n-1)*(n+1)]
    */
    /* compute a(n-3) recursively (memoised) */
    seq_t res = a(n - 3);

    /* numerator factors */
    seq_t numf[4];
    numf[0] = (seq_t)216;
    numf[1] = (seq_t)(4 * n - 5);
    numf[2] = (seq_t)(2 * n - 1);
    numf[3] = (seq_t)(4 * n - 11);

    /* denominator factors */
    seq_t den[3];
    den[0] = (seq_t)n;
    den[1] = (seq_t)(n - 1);
    den[2] = (seq_t)(n + 1);

    /* multiply numerators while reducing with denominators via gcd */
    for (size_t i = 0; i < 4; ++i) {
        res *= numf[i];
        for (size_t j = 0; j < 3; ++j) {
            seq_t g = gcd(res, den[j]);
            if (g > 1) {
                res /= g;
                den[j] /= g;
            }
        }
    }

    /* any remaining denominators must divide evenly */
    for (size_t j = 0; j < 3; ++j) {
        if (den[j] != (seq_t)1)
            res /= den[j];
    }

    /* store and return */
    cache[n] = res;
    return res;
}
