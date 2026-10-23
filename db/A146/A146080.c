/*
 * A146080  Expansion of 1/(1-x*(1-10*x)).
 * https://oeis.org/A146080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 10*a(n-2), a(0)=1, a(1)=1
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 19, i64 n <= 37, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for idx_t and seq_t definitions */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;

    seq_t a0 = (seq_t)1;            /* a(n-2) */
    seq_t a1 = (seq_t)1;            /* a(n-1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = a1 - (seq_t)10 * a0;   /* a(i) = a(i-1) - 10*a(i-2) */
        a0 = a1;
        a1 = an;
    }
    return a1;
}
