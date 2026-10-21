/*
 * A062482  Integer part of (Product(n^((1 + log(i))/i^2), {i, 1, n})).
 * https://oeis.org/A062482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Floor[ n^Sum_{i=1..n} (1 + Log[i]) / i^2 ]
 * Range:     u8 n <= 12, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The product over i of n^((1+Log[i])/i^2) equals n^Sum_{i=1..n} (1+Log[i])/i^2. */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* The build system supplies seq_t and idx_t, do not #define them here. */

seq_t a(idx_t n)
{
    /* Empty product (n=0) is 1. */
    if (n == 0)
        return (seq_t)1;

    /* Static cache of the partial sums Σ_{i=1..k} (1+Log[i])/i^2. */
    static long double *sum = NULL;   /* sum[i] holds the sum up to i */
    static size_t cap = 0;            /* number of elements allocated */

    /* Grow the cache if necessary. */
    if (n >= cap) {
        size_t new_cap = (cap == 0) ? 1 : cap * 2;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        long double *tmp = (long double *)realloc(sum, new_cap * sizeof(long double));
        if (tmp == NULL) {
            /* Out of memory – return 0 as a safe fallback. */
            return (seq_t)0;
        }
        sum = tmp;
        if (cap == 0)
            sum[0] = 0.0L;               /* Σ up to 0 is 0 */
        for (size_t i = cap; i < new_cap; ++i)
            sum[i] = 0.0L;
        cap = new_cap;
    }

    /* Compute the sums that are still missing. */
    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (sum[i] == 0.0L) {
            long double li = logl((long double)i);
            sum[i] = sum[i - 1] + (1.0L + li) / ((long double)i * (long double)i);
        }
    }

    /* n^sum[n] = exp( sum[n] * ln(n) ) */
    long double exponent = sum[(size_t)n];
    long double ln_n     = logl((long double)n);
    long double val      = expl(exponent * ln_n);

    /* Guard against floating‑point inaccuracies. */
    static const long double eps = 1e-12L;
    unsigned long long result = (unsigned long long)floorl(val + eps);

    return (seq_t)result;
}
