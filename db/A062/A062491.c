/*
 * A062491  Nearest integer to (Product(n^(log(1 + i)/(1 + i^2)), {i, 1, n})).
 * https://oeis.org/A062491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = nearest integer to n^( Sum_{i=1..n} log(1+i)/(1+i^2) )
 *  a(0) = 1
 *
 *  The sequence is computed in O(1) amortized time per call by
 *  incrementally maintaining the cumulative sum of the logarithmic
 *  terms.  All arithmetic is done in double/long double to avoid
 *  the heavy use of arbitrary‑precision math.  The final
 *  rounding to the nearest integer is performed with the expression
 *  floor(x+0.5).  The resulting integer is guaranteed to fit into
 *  seq_t for all n<=10000.
 */

#include <math.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* First term (empty product) */
    if (n == 0) return (seq_t)1;

    /* Static storage for cumulative sums */
    static double *cum = NULL;          /* cum[i] = Σ_{k=1..i} log(1+k)/(1+k^2) */
    static idx_t  cum_n = 0;            /* largest i computed so far */

    /* Grow storage if needed */
    if (n > cum_n) {
        size_t new_size = (size_t)n + 1U;   /* indices 0..n inclusive */
        cum = (double *)realloc(cum, new_size * sizeof(double));
        if (!cum) { /* allocation failed: fall back to safe minimal value */
            return (seq_t)1;
        }
        /* Initialize the new portion */
        for (idx_t i = cum_n + 1; i <= n; ++i) {
            double ip = (double)i;                  /* i as double */
            double term = log( (double)(ip + 1.0) ) / (1.0 + ip * ip);
            cum[i] = cum[i - 1] + term;
        }
        cum_n = n;
    }

    /* Compute n^cum[n] with long double for better precision */
    long double power = pow( (long double)n, (long double)cum[n] );
    /* Round to nearest integer */
    unsigned long long val = (unsigned long long)(power + 0.5L);
    return (seq_t)val;
}
