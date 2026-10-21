/*
 * A091573  Poincaré series [or Poincare series] of the preprojective algebra
 *          of an extended Dynkin diagram of type E_6.
 * https://oeis.org/A091573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*n+7 if n%4==0, 6*n+5 if n%4==2, 6*n+6 if n is odd
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 6;                   /* 6*n */
    if ((n & 1) == 1)                          /* n odd */
        return base + 6;
    else if ((n & 3) == 0)                     /* n ≡ 0 (mod 4) */
        return base + 7;
    else                                       /* n ≡ 2 (mod 4) */
        return base + 5;
}
