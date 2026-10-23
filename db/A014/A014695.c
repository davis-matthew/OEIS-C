/*
 * A014695  Poincaré series [or Poincare series] (or Molien series) for mod 2
 *          cohomology of Q_8.
 * https://oeis.org/A014695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) repeats the pattern 1, 2, 2, 1 (period 4)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pattern[4] = {(seq_t)1, (seq_t)2, (seq_t)2, (seq_t)1};
    return pattern[n & 3ULL];
}
