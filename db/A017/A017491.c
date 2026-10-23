/*
 * A017491  a(n) = (11*n + 8)^7.
 * https://oeis.org/A017491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 8)^7
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 50, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute base = 11*n + 8 */
    seq_t base = ((seq_t)11) * ((seq_t)n) + ((seq_t)8);

    /* Compute base^7 by repeated multiplication */
    seq_t result = base;   /* base^1 */
    for (int i = 1; i < 7; ++i)  /* multiply 6 more times to get exponent 7 */
    {
        result *= base;
    }
    return result;
}
