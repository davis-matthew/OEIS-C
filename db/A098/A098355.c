/*
 * A098355  Multiplication table of the powers of three read by antidiagonals.
 * https://oeis.org/A098355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3^k , where k = floor( (sqrt(8*n + 1) - 1) / 2 )
 *            Equivalently, k is the smallest integer such that n <
 *            (k+1)(k+2)/2. This yields 3^n repeated n+1 times (the sequence
 *            A098355)
 * Range:     u8 n <= 20, u16 n <= 65, u32 n <= 230, u64 n <= 350,
 *            u128 n <= 350, big any n
 * Helpers:   pow3_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow3_uint.h"

seq_t a(idx_t n)
{
    /* Find k such that n < (k+1)(k+2)/2.
       Since n ≤ 10000 in the tests, a simple linear search over k is more than
       fast enough (k is ≲ 140). */
    idx_t k = 0;
    while ( ((k + 1) * (k + 2)) / 2 <= n ) {
        ++k;
    }

    return pow3_uint(k);
}
