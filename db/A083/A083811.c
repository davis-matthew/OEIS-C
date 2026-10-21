/*
 * A083811  Numbers k such that 2*k+1 is the digit reversal of k+1.
 * https://oeis.org/A083811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 4*(10^n - 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, return 0 */
    seq_t pow10 = (seq_t)10;       /* 10^1 */
    for (idx_t i = 1; i < n; ++i)  /* compute 10^n */
        pow10 *= (seq_t)10;
    seq_t result = pow10 - (seq_t)1;
    result *= (seq_t)4;
    return result;
}
