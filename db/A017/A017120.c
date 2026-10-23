/*
 * A017120  a(n) = (8*n + 4)^8.
 * https://oeis.org/A017120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 4)^8
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 31, u128 n <= 8191,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute base = 8*n + 4 */
    seq_t base = ((seq_t)n * (seq_t)8) + (seq_t)4;

    /* Compute base^8 by repeated multiplication (constant exponent). */
    seq_t result = base;
    for (idx_t i = 1; i < 8; ++i)
        result = result * base;   /* seq_t supports ordinary multiplication */

    return result;
}
