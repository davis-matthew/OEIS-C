/*
 * A017411  a(n) = (11*n + 1)^11.
 * https://oeis.org/A017411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 1)^11
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 5, u128 n <= 289,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    seq_t base = ((seq_t)n) * 11 + 1;
    seq_t b2   = base * base;   /* base^2            */
    seq_t b4   = b2   * b2;     /* base^4            */
    seq_t b8   = b4   * b4;     /* base^8            */
    seq_t res  = b8   * b2;     /* base^10           */
    res        = res  * base;   /* base^11           */
    return res;
}
