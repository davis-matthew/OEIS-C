/*
 * A169634  a(n) = 3*7^n.
 * https://oeis.org/A169634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*7^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 3 * 7^n using a simple loop.
       For n = 0 the loop does not execute and the
       result is 3, as required. */
    seq_t result = (seq_t)3;
    for (idx_t i = 1; i <= n; ++i) {
        result *= (seq_t)7;
    }
    return result;
}
