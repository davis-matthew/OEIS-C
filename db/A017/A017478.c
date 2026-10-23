/*
 * A017478  a(n) = (11*n + 7)^6.
 * https://oeis.org/A017478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 7)^6
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 147, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert n to seq_t and compute the base value 11*n + 7 */
    seq_t base = ((seq_t)n * 11) + 7;

    /* Compute powers: base^2, base^4, base^6 */
    seq_t base2 = base * base;          /* base^2 */
    seq_t base4 = base2 * base2;        /* base^4 */
    seq_t result = base4 * base2;       /* base^6 */

    return result;
}
