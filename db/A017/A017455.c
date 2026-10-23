/*
 * A017455  a(n) = (11*n + 5)^7.
 * https://oeis.org/A017455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 5)^7
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 50, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute base = 11*n + 5 and then raise it to the 7th power. */
    seq_t base = ((seq_t)11 * (seq_t)n) + 5;
    seq_t result = 1;
    for (int i = 0; i < 7; ++i)
    {
        result *= base;
    }
    return result;
}
