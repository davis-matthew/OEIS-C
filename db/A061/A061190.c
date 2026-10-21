/*
 * A061190  a(n) = n^n - n.
 * https://oeis.org/A061190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n - n, with 0^0 defined as 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;              /* 0^0 is defined as 1 in this sequence */

    seq_t base = (seq_t)n;
    seq_t p    = pow_int(base, n);    /* compute n^n */
    return p - base;                  /* subtract n */
}
