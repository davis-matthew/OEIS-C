/*
 * A122959  a(0) = 1, a(n) = (-1)^n*(2-2^(n-1)) for n>0.
 * https://oeis.org/A122959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n*(2-2^(n-1)) for n>0, a(0)=1
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(0)=1, a(1)=-1, a(2)=0, a(n)=(-3)*a(n-1)-2*a(n-2) for n>2 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t power = pow2(n - 1);           /* 2^(n-1) */
    seq_t result = (seq_t)2 - power;     /* 2 - 2^(n-1) */

    if (n & ((idx_t)1))                  /* multiply by (-1)^n */
        result = -result;

    return result;
}
