/*
 * A062480  Nearest integer to log(n^n)^(1 + log(log(1 + n))).
 * https://oeis.org/A062480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to ( log(n^n) )^( 1 + log( log(1 + n) ) )
 *            for n ≥ 1
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Sequence is defined only for n ≥ 1 */
    if (n < 1) return 0;

    /* natural logarithm of n */
    long double ln_n = logl((long double)n);

    /* log(n^n) = n * log(n) */
    long double base = (long double)n * ln_n;

    /* log(log(1 + n)) */
    long double inner = logl(1.0L + (long double)n);
    long double exponent = 1.0L + logl(inner);

    /* (log(n^n)) ^ (1 + log(log(1 + n))) */
    long double val = powl(base, exponent);

    /* nearest integer to the result */
    long double rounded = roundl(val);

    /* Convert the rounded value to an integer that seq_t can store */
    unsigned long long ull = (unsigned long long)rounded;
    return (seq_t)ull;
}
