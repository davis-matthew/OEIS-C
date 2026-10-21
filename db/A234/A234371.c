/*
 * A234371  Bohr-Procopiu magneton.
 * https://oeis.org/A234371
 *
 * Offset:    -23    (a(-23) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(9.2740100783 * 10^(n+23)) mod 10 The first 11
 *            digits are 9,2,7,4,0,1,0,0,7,8,3; all other terms are 0
 * Range:     u8 n <= -16, u16 n <= -16, u32 n <= -16, u64 n <= -16,
 *            u128 n <= -16, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t i = n + 23;                     /* position of the digit in the mantissa 9.2740100783 */
    if (i < 0) return (seq_t)0;           /* n < -23: out of defined range, return 0 */
    static const unsigned char digits[11] = {9,2,7,4,0,1,0,0,7,8,3};
    if (i < 11) return (seq_t)digits[i];
    return (seq_t)0;                      /* all subsequent digits are zero */
}
