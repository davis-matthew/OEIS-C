/*
 * A116952  a(n) = 3*a(n-1) + 5 with a(0) = 1.
 * https://oeis.org/A116952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 5, with a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;       /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
        result = (seq_t)3 * result + (seq_t)5;
    return result;
}
