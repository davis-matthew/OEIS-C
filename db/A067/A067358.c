/*
 * A067358  Imaginary part of (5+12i)^n.
 * https://oeis.org/A067358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, frac
 * Formula:   a(n) = 10*a(n-1) - 169*a(n-2); a(0) = 0, a(1) = 12
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 17, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)12;

    seq_t a0 = (seq_t)0;     /* a(n-2) */
    seq_t a1 = (seq_t)12;    /* a(n-1) */
    seq_t an = a1;           /* placeholder for current value */

    for (idx_t i = 2; i <= n; ++i)
    {
        an = a1 * (seq_t)10 - a0 * (seq_t)169;
        a0 = a1;
        a1 = an;
    }
    return an;
}
