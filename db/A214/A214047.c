/*
 * A214047  Least m>0 such that n! <= (3/2)^m.
 * https://oeis.org/A214047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least m>0 such that n! <= (3/2)^m
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>

/* Dynamic table of log10(k!) values, built lazily. */
static double *logfact = NULL; /* logfact[i] == log10(i!) for 0 <= i <= logfact_max */
static idx_t   logfact_max = 0;

/* Ensure that logfact contains entries up to n. */
static void ensure_logfact(idx_t n)
{
    if (n <= logfact_max) return;

    /* Allocate or grow the table. */
    size_t newcap = (size_t)n + 1;
    double *newarr = (double *)malloc(newcap * sizeof(double));
    if (!newarr) abort();                /* allocation failure */

    /* Initialise first entry if the table was empty. */
    if (logfact == NULL) {
        newarr[0] = 0.0;                 /* log10(0!) = 0 */
    } else {
        /* Copy existing values. */
        for (idx_t i = 0; i <= logfact_max; ++i)
            newarr[i] = logfact[i];
    }

    /* Compute new entries. */
    for (idx_t i = logfact_max + 1; i <= n; ++i)
        newarr[i] = newarr[i - 1] + log10((double)i);

    /* Replace old table. */
    free(logfact);
    logfact = newarr;
    logfact_max = n;
}

/* Log10 of 3/2. */
static const double LOG10_3_2 = 0.176091259055681242; /* log10(1.5) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n<1, tests start at 1 */

    /* Build log10 factorial table up to n. */
    ensure_logfact(n);
    double logfact_n = logfact[n];

    /* Estimate m via logarithms.  Add a tiny epsilon to guard against rounding. */
    double est = logfact_n / LOG10_3_2;
    idx_t m = (idx_t)ceil(est - 1e-12);

    /* Adjust m so that (3/2)^(m-1) < n! <= (3/2)^m. */
    /* We use the logarithmic inequality to avoid huge exponentiation. */
    while (( (double)(m-1) * LOG10_3_2 ) >= logfact_n && m > 0) {
        --m;
    }
    while ( ( (double)m * LOG10_3_2 ) < logfact_n ) {
        ++m;
    }

    /* For n=1 the formula yields m=0; the sequence requires m>0. */
    if (m == 0) m = 1;

    return (seq_t)m;
}
