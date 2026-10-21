/*
 * A079685  Degrees of irreducible representations of SL(2,5).
 * https://oeis.org/A079685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 1 for n=1; a(n) = 2 for n=2,3; a(n) = 3 for n=4,5; a(n) =
 *            4 for n=6,7; a(n) = 5 for n=8; a(9) = 6; a(n) = 0 for n>9
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    switch ((idx_t)n) {
    case 1:  return (seq_t)1;
    case 2:  return (seq_t)2;
    case 3:  return (seq_t)2;
    case 4:  return (seq_t)3;
    case 5:  return (seq_t)3;
    case 6:  return (seq_t)4;
    case 7:  return (seq_t)4;
    case 8:  return (seq_t)5;
    case 9:  return (seq_t)6;
    default: return (seq_t)0;
    }
}
