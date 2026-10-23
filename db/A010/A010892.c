/*
 * A010892  Inverse of 6th cyclotomic polynomial. A period 6 sequence.
 * https://oeis.org/A010892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = a(n-1) - a(n-2) with a(0)=1, a(1)=1. Equivalent periodic
 *            representation: a(n)=1 if n%6=0 or 1, a(n)=0 if n%6=2 or 5, and
 *            a(n)=-1 otherwise
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    switch (n % 6) {
    case 0:
    case 1:
        return (seq_t)1;
    case 2:
    case 5:
        return (seq_t)0;
    default: /* case 3 or 4 */
        return (seq_t)-1;
    }
}
