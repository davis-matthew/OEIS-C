/*
 * A047929  a(n) = n^2*(n-1)*(n-2).
 * https://oeis.org/A047929
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*n*(n-1)*(n-2)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence has offset 2, but for n<2 the function is not
       required to produce any particular value.  We simply return 0
       for such inputs to avoid underflow from unsigned subtraction. */
    if (n < 2)
        return (seq_t)0;

    /* Convert the index to the target integer type first,
       then perform the arithmetic using only seq_t operations. */
    seq_t tn = (seq_t)n;

    return tn * tn * (tn - 1) * (tn - 2);
}
