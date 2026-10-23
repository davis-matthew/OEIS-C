/*
 * A034112  Fractional part of square root of a(n) starts with 6: first term
 *          of runs.
 * https://oeis.org/A034112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ceil((10n+16)^2 / 100)
 * Range:     u8 none, u16 n <= 23, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1; behaviour for n < 1 does not matter */
    if (n == 0)
        return 0;

    /* work in seq_t to avoid overflow and to support arbitrary precision */
    seq_t tn = (seq_t)n;        /* convert index to seq_t */
    seq_t t  = tn * (seq_t)10 + (seq_t)16;   /* t = 10 * n + 16 */
    seq_t x  = t * t;                                /* x = t^2  */
    return (x + (seq_t)99) / (seq_t)100;            /* ceil(x/100) */
}
