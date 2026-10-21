/*
 * A057640  a(n) = floor(H(n) + exp(H(n))*log(H(n))), where H(n) =
 *          Sum_{k=1..n} 1/k.
 * https://oeis.org/A057640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = floor( H(n) + exp(H(n)) * log(H(n)) ) with H(n) =
 *            sum_{k=1..n} 1/k. The implementation uses a static table of
 *            harmonic numbers stored as long double. Each time the function
 *            is called the table is extended up to the requested index. The
 *            final value is evaluated in long double and then cast...
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

/* Harmonic number cache, indexed by size_t (same as idx_t on 64‑bit). */
static long double *harmonic_cache = NULL;
static size_t harmonic_size = 0;

seq_t a(idx_t n)
{
    /* n is at least 1 in OEIS usage, but guard anyway. */
    if (n == 0)
        return (seq_t)0;

    size_t req = (size_t)n;

    /* Initialise cache on first call. */
    if (harmonic_size == 0) {
        harmonic_cache = (long double *)malloc(sizeof(long double));
        if (!harmonic_cache)
            return (seq_t)0;                     /* out‑of‑memory fallback */
        harmonic_cache[0] = 0.0L;                /* 0-th harmonic is unused */
        harmonic_size  = 1;
    }

    /* extend cache if required */
    if (req >= harmonic_size) {
        size_t new_size = req + 1;
        long double *tmp = (long double *)realloc(harmonic_cache,
                                                 new_size * sizeof(long double));
        if (!tmp)
            return (seq_t)0;                    /* allocation failure fallback */
        harmonic_cache = tmp;
        for (size_t i = harmonic_size; i <= req; ++i)
            harmonic_cache[i] = harmonic_cache[i-1] + 1.0L / (long double)i;
        harmonic_size = new_size;
    }

    /* compute the formula in long double */
    long double H = harmonic_cache[req];
    long double val = H + expl(H) * logl(H);

    /* floor with a tiny epsilon to guard against rounding errors */
    const long double eps = 1e-12L;
    unsigned long long tmp = (unsigned long long)floorl(val + eps);

    return (seq_t)tmp;
}
