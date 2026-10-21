/*
 * A091574  Poincaré series [or Poincare series] of the preprojective algebra
 *          of an extended Dynkin diagram of type D_4.
 * https://oeis.org/A091574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*(n+1) if n even, 4*(n+1) if n odd
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0)
        return (seq_t)5 * ((seq_t)n + 1);
    else
        return (seq_t)4 * ((seq_t)n + 1);
}
