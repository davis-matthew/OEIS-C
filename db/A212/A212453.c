/*
 * A212453  a(n) = ceiling(4n + log(4n)).
 * https://oeis.org/A212453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n + ceil(log(4*n))
 * Range:     u8 n <= 62, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* 4*n as seq_t, to keep arithmetic in the integer domain */
    seq_t term = (seq_t)4 * (seq_t)n;

    /* compute ceil(log(4*n)) using long double arithmetic */
    long double v = (long double)4 * (long double)n;
    long double ln = logl(v);                   /* natural logarithm */
    unsigned long long ln_ceil = (unsigned long long)ceill(ln); /* integer ceiling */

    term += (seq_t)ln_ceil;                     /* add the integer part */
    return term;
}
