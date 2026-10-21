/*
 * A062444  Nearest integer to log(n!)^(1 + log(1 + log(n))).
 * https://oeis.org/A062444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to ( log(n!) )^( 1 + log(1 + log(n)) )
 *            Indexing: offset 0 (i.e., a(0) is the first term)
 * Range:     u8 n <= 9, u16 n <= 32, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Trivial cases that avoid undefined logarithms */
    if (n <= 1) return 0;

    /* Static table of natural logarithms of factorials:
       logfact[i] == ln(i!)  (i = 0..current_size-1)                    */
    static long double *logfact = NULL;
    static size_t      size    = 0;        /* current table length */

    /* Extend the table up to index n (inclusive) if necessary */
    if (size <= (size_t)n)
    {
        size_t newsize = (size_t)n + 1;

        /* Reallocate memory (C requires cast, C++ can use it directly) */
        long double *tmp = (long double *)realloc(logfact,
                                                  newsize * sizeof(long double));
        if (!tmp) return 0;          /* allocation failure → return 0 (fail‑safe) */
        logfact = tmp;

        /* Initialisation of the table */
        if (size == 0)
        {
            logfact[0] = 0.0L;       /* ln(0!) = ln(1) = 0 */
            logfact[1] = 0.0L;       /* ln(1!) = 0 */
            size       = 2;
        }

        /* Compute successive logarithms of factorials */
        for (size_t i = size; i <= (size_t)n; ++i)
            logfact[i] = logfact[i - 1] + logl((long double)i);
        size = newsize;
    }

    /* Compute the expression from the definition */
    long double ln_fact   = logfact[n];
    long double exponent  = 1.0L + logl(1.0L + logl((long double)n));
    long double value     = powl(ln_fact, exponent);

    /* Return the nearest integer; automatically rounds towards nearest */
    return (seq_t)llround(value);
}
