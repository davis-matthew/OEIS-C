/*
 * A136516  a(n) = (2^n+1)^n.
 * https://oeis.org/A136516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n + 1)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow2, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t twop = pow2(n);

    /* Base = 2^n + 1 */
    seq_t base = twop + (seq_t)1;

    /* Return (2^n + 1)^n */
    return seq_pow(base, n);
}
