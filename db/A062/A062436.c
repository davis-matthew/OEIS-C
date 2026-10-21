/*
 * A062436  Nearest integer to log(n!)^log(log(1 + n)).
 * https://oeis.org/A062436
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A062436
    a(n) = nearest integer to (log(n!))^(log(log(1+n)))   (n >= 2)
    Offest: 2 (a(2) is the first term)
    The function uses long double arithmetic for the logarithms
    and then rounds to the nearest integer.  The result is cast to
    seq_t; all calculations that involve seq_t values are done
    with integer arithmetic only. */

#include <math.h>          /* logl, powl, llroundl */
#include <stddef.h>        /* size_t */

seq_t a(idx_t n)
{
    if (n < 2)              /* behaviour below offset is irrelevant */
        return (seq_t)0;

    /* static cache of log factorials */
    static size_t cache_size = 1;           /* currently cached up to logfac[1] */
    static long double *logfac = NULL;      /* logfac[i] == log(i!) */

    /* initialise cache on first call */
    if (logfac == NULL) {
        logfac = (long double *)malloc(sizeof(long double) * 2);
        cache_size = 2;
        logfac[0] = 0.0L;           /* log(0!) = 0 */
        logfac[1] = 0.0L;           /* log(1!) = 0 */
    }

    /* extend cache up to required n */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;          /* need indices 0..n */
        logfac = (long double *)realloc(logfac, sizeof(long double) * new_size);
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            logfac[i] = logfac[i - 1] + logl((long double)i);
        }
        cache_size = new_size;
    }

    long double log_fact = logfac[n];               /* log(n!)  */
    long double loglog = logl( logl( (long double)(n + 1) ) );  /* log(log(n+1)) */

    long double value = powl(log_fact, loglog);     /* real value */

    /* round to nearest integer */
    seq_t result = (seq_t) llroundl(value);

    return result;
}
