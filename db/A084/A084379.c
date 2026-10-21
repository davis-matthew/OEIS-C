/*
 * A084379  a(n) = n^3 + 17.
 * https://oeis.org/A084379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^3 + 17
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute n^3 + 17 using seq_t arithmetic */
    seq_t nn = (seq_t)n;
    return nn * nn * nn + (seq_t)17;
}
