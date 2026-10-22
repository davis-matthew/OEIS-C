/*
 * A002887  The minimum number of nodes of a tree with a cutting center of n
 *          nodes.
 * https://oeis.org/A002887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 3 (n=1), 4 (n=2), 7 (n=3); a(n) = 10 * n!/4! for n >= 4
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
  if (n == 1) return (seq_t)3;
  if (n == 2) return (seq_t)4;
  if (n == 3) return (seq_t)7;
  if (n == 4) return (seq_t)10;
  
  seq_t result = (seq_t)10;
  for (idx_t i = 5; i <= n; i++) {
    result *= (seq_t)i;
  }
  return result;
}
