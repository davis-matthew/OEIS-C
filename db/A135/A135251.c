/*
 * A135251  Maximal number of zero digits in square of number with n digits
 *          not divisible by 10.
 * https://oeis.org/A135251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1)=0, a(2)=1, a(3)=3, a(n)=2*n-4 for n≥4
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)          return (seq_t)0;
    if (n == 2)          return (seq_t)1;
    if (n == 3)          return (seq_t)3;
    /* for n≥4 */
    const seq_t nn = (seq_t)n;
    return nn * (seq_t)2 - (seq_t)4;
}
