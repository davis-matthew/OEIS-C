/*
 * A000460  Eulerian numbers (Euler's triangle: column k=3 of A008292, column
 *          k=2 of A173018).
 * https://oeis.org/A000460
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - (n+1)*2^n + n*(n+1)/2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   a460_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a460_pow.h"

seq_t a(idx_t n)
{
  seq_t pow3 = a460_pow((seq_t)3, n);
  seq_t pow2 = a460_pow((seq_t)2, n);
  seq_t term2 = ((seq_t)n + 1) * pow2;
  seq_t term3;
  if (n % 2 == 0)
    term3 = (seq_t)(n / 2) * ((seq_t)n + 1);
  else
    term3 = (seq_t)n * (seq_t)((n + 1) / 2);
  return pow3 + term3 - term2;
}
