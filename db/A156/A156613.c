/*
 * A156613  First nonzero digit in decimal expansion in (e^n)/n!.
 * https://oeis.org/A156613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of a(n): first non‑zero digit of (e^n)/n! */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <cmath>
#else
#include <math.h>
#endif

/* Helper macros that work both in C and C++ */
#ifdef __cplusplus
#define LOG10  std::log10
#define FLOOR std::floor
#define POW   std::pow
#else
#define LOG10  log10
#define FLOOR  floor
#define POW    pow
#endif

static double *logsum = 0;          /* cumulative sums of log10(k) */
static idx_t  logsum_len = 0;       /* number of entries stored */
static const double log10e = 0.43429448190325182765112891891660508;

seq_t a(idx_t n)
{
    /* Extend the cache so that logsum[n] exists */
    if (n >= logsum_len) {
        size_t new_len = (size_t)n + 1;                 /* array size needed */
        double *tmp = (double*)realloc(logsum, new_len * sizeof(double));
        if (!tmp) return 0;                            /* memory failure unlikely */

        if (logsum_len == 0)                            /* base case */
            tmp[0] = 0.0;

        for (idx_t i = logsum_len; i <= n; ++i) {
            if (i == 0)
                tmp[i] = 0.0;
            else
                tmp[i] = tmp[i - 1] + LOG10((double)i);
        }
        logsum = tmp;
        logsum_len = new_len;
    }

    /* log10( (e^n)/n! ) = n*log10(e) - Σ_{k=1}^{n} log10(k) */
    double log10_val = (double)n * log10e - logsum[n];

    /* fractional part gives the leading digit */
    double frac = FLOOR(log10_val);
    double f = log10_val - frac;
    double p = POW(10.0, f);

    int d = (int)(p + 1e-12);   /* small epsilon protects against 9.999… */

    if (d >= 10) d = 9;   /* safety against rounding to 10.0 */
    if (d < 1)   d = 1;
    return (seq_t)d;
}
