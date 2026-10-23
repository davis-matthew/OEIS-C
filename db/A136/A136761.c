/*
 * A136761  a(n) = leading digit of n! in base 11.
 * https://oeis.org/A136761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = leading digit of n! in base 11 (0‑based)
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* Caches cumulative log₁₁ of k! for 0 ≤ k ≤ cache_max. */
static idx_t cache_max = 0;          /* largest k cached */
static long double *logsum = NULL;   /* logsum[0] = 0 */
static const long double LOG11 = logl(11.0L);

/* Return the leading base‑11 digit of n!. The digit is an integer 1…10. */
seq_t a(idx_t n)
{
    /* 0! = 1 → leading digit 1 */
    if (n == 0)
        return (seq_t)1;

    /* Initialize cache if needed */
    if (logsum == NULL)
    {
        logsum = (long double *)malloc(sizeof(long double));
        if (!logsum)
            abort();
        logsum[0] = 0.0L;
        cache_max = 0;
    }

    /* Grow cache to include n */
    if (n > cache_max)
    {
        size_t new_size = (size_t)n + 1;
        long double *tmp = (long double *)realloc(logsum, new_size * sizeof(long double));
        if (!tmp)
            abort();
        logsum = tmp;
        for (idx_t i = cache_max + 1; i <= n; ++i)
        {
            logsum[i] = logsum[i - 1] + logl((long double)i) / LOG11;
        }
        cache_max = n;
    }

    /* Extract fractional part of log₁₁(n!) */
    long double value = logsum[n];
    long double frac = value - floorl(value);

    /* Leading digit = ⌊11^frac⌋, add tiny epsilon to avoid 10.999… */
    long double lead = powl(11.0L, frac) + 1e-12L;
    unsigned long long digit = (unsigned long long)lead; /* 1 … 10 */

    return (seq_t)digit;
}
