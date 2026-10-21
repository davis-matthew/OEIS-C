/*
 * A062437  Integer part of log(n^n)^log(log(1 + n)).
 * https://oeis.org/A062437
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (n*log(n)) ^ (log(log(1+n))) )
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (using natural logarithm in all logarithms) */
#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* offset is 2; below does not matter */
    long double nd = (long double)n;
    /* B = n * log(n)          */
    long double B = nd * logl(nd);
    /* E = log(log(1+n))      */
    long double E = logl(logl(nd + 1.0L));
    /* logA = E * log(B)      */
    long double logA = E * logl(B);
    /* A = exp(logA)          */
    long double A = expl(logA);
    /* integer part */
    unsigned long long val = (unsigned long long)floorl(A + 1e-12L);
    return (seq_t)val;
}
