/*
 * A003308  a(n) = 2*n^(n-2).
 * https://oeis.org/A003308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n^(n-2), with a(1) = 2
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   a003308_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a003308_ipow.h"

seq_t a(idx_t n)
{
  if (n <= 1)
    return 2;
  return (seq_t)2 * a003308_ipow((seq_t)n, n - 2);
}
