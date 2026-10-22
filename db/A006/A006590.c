/*
 * A006590  a(n) = Sum_{k=1..n} ceiling(n/k).
 * https://oeis.org/A006590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{k=1..n} ceiling(n/k)
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
  seq_t nn = (seq_t)n;
  seq_t result = 0;
  for (seq_t k = 1; k <= nn; k++) {
    result += (nn + k - 1) / k;
  }
  return result;
}
