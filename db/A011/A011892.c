/*
 * A011892  a(n) = floor( n*(n-1)*(n-2)/10 ).
 * https://oeis.org/A011892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(n-1)*(n-2)/10 )
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 3 the product contains a zero factor, so the result is 0. */
    if (n < 3)
        return 0;

    /* Convert the index to seq_t to keep all arithmetic within the
       desired integer type.  This is safe because n is small enough
       that the intermediate products fit in the type. */
    seq_t nn = (seq_t)n;

    /* Compute the product n*(n-1)*(n-2) using unsigned arithmetic. */
    seq_t prod = nn * (nn - 1);
    prod = prod * (nn - 2);

    /* Divide by 10.  Integer division truncates towards zero, giving
       the floor for all positive results. */
    prod = prod / 10;

    return prod;
}
