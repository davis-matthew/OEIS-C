/*
 * A017059  a(n) = (7*n + 6)^7.
 * https://oeis.org/A017059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 6)^7
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 79, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t base = (seq_t)7 * n + (seq_t)6;  /* compute 7*n + 6 as seq_t */
    seq_t result = base;
    for (int i = 1; i < 7; ++i) {
        result *= base;                    /* multiply 7 times to get the 7th power */
    }
    return result;
}
