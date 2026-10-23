/*
 * A158062  a(n) = 36*n^2 - 2*n.
 * https://oeis.org/A158062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*n^2 - 2*n (offset 1)
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t nn = (seq_t)n;
    /* compute 36*n^2 - 2*n as nn * (36*nn - 2) */
    return nn * ((((seq_t)36) * nn) - ((seq_t)2));
}
