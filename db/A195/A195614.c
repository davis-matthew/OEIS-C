/*
 * A195614  Denominators a(n) of Pythagorean approximations b(n)/a(n) to 2.
 * https://oeis.org/A195614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) + 17*a(n-2) - a(n-3) with initial values
 *            a(1)=8, a(2)=136, a(3)=2448
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* handle offsets below 1; unused for the OEIS data */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Initial three terms */
    seq_t a1 = (seq_t)8;       /* a(1) */
    seq_t a2 = (seq_t)136;     /* a(2) */
    seq_t a3 = (seq_t)2448;    /* a(3) */

    if (n == 1) { return a1; }
    if (n == 2) { return a2; }
    if (n == 3) { return a3; }

    /* Iteratively compute up to a(n) */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)17 * a3 + (seq_t)17 * a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;   /* now a3 holds a(n) */
}
