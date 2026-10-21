/*
 * A091577  Poincaré series [or Poincare series] of the preprojective algebra
 *          of a Dynkin diagram of type E_6.
 * https://oeis.org/A091577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n > 10 a(n) = a(10 - n) for 7 ≤ n ≤ 10 a(n) = 6 +
 *            4*n (n ≥ 0, n ≤ 3) for 0 ≤ n ≤ 3 a(n) = 20 for 4 ≤ n ≤ 6 (the
 *            sequence is palindromic, symmetric about n = 5)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 10)  return (seq_t)0;
    if (n >= 6)  n = 10 - n;          /* use symmetry */
    if (n <= 3)  return (seq_t)6 + (seq_t)4 * n;
    return (seq_t)20;                 /* for n = 4,5,6 */
}
