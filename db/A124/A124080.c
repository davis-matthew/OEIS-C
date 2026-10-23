/*
 * A124080  10 times triangular numbers: a(n) = 5*n*(n + 1).
 * https://oeis.org/A124080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*n*(n+1)
 * Range:     u8 n <= 6, u16 n <= 113, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cast the index to the sequence type first to avoid
       intermediate 32‑bit overflow when seq_t is wider.  */
    seq_t nn = (seq_t)n;
    return nn * (nn + 1) * 5;
}
