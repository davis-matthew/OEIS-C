/*
 * A017490  a(n) = (11*n + 8)^6.
 * https://oeis.org/A017490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 8)^6
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 147, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t base  = ((seq_t)n * 11) + 8;
    seq_t value = base * base;   /* base^2  */
    value *= base;               /* base^3  */
    value *= base;               /* base^4  */
    value *= base;               /* base^5  */
    value *= base;               /* base^6  */
    return value;
}
