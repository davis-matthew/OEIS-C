/*
 * A062414  Nearest integer to log(n!)^(1 - 1/n).
 * https://oeis.org/A062414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to (log(n!))^(1 - 1/n)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* To compute a(n) numerically, we precompute
   logarithms of factorials in a long‑double table, then
   evaluate (log(n!))^(1-1/n) with long‑double arithmetic.
   The final result is rounded and returned as seq_t. */

#include <stdlib.h>
#include <math.h>

/* Local table of cumulative logarithms of factorials: logfact[i] = log(i!)  */
static long double *logfact = NULL;   /* pointer to the table          */
static idx_t logfact_cap   = 0;       /* allocated capacity (>= n+1)  */
static idx_t logfact_last  = 0;       /* largest index computed so far */

/* Ensure that logfact[i] is available for all i <= n. */
static void ensure_logfact(idx_t n)
{
    if (n <= logfact_last)
        return;                           /* already computed */

    /* Allocate or grow the table. */
    if (logfact == NULL) {
        logfact_cap = n + 2;               /* at least n+1 entries   */
        logfact = (long double *)malloc(logfact_cap * sizeof(long double));
        if (!logfact) abort();
        logfact[0] = 0.0L;                /* log(0!) = 0            */
        logfact_last = 0;
    } else if (logfact_cap <= (size_t)n + 1) {
        logfact_cap = (n + 2) * 2;
        long double *tmp = (long double *)realloc(logfact, logfact_cap * sizeof(long double));
        if (!tmp) abort();
        logfact = tmp;
    }

    /* Extend the table up to n. */
    long double cur = logfact[logfact_last];
    for (idx_t i = logfact_last + 1; i <= n; ++i) {
        cur += log((long double)i);
        logfact[i] = cur;
    }
    logfact_last = n;
}

/* Compute a(n).  n must be ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)          /* defined for safety */
        return (seq_t)0;

    /* Special case n == 1: log(1!) = 0, 0^(1-1) = 0^0 is treated as 1 */
    if (n == 1)
        return (seq_t)1;

    ensure_logfact(n);

    long double L   = logfact[n];                     /* L = log(n!)          */
    long double exp = 1.0L - (1.0L / (long double)n);  /* 1 - 1/n              */
    long double val = pow(L, exp);                    /* L^(1-1/n)             */

    /* Round to nearest integer and cast to seq_t. */
    long long rounded = (long long)(val + 0.5L);
    return (seq_t)rounded;
}
