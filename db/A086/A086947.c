/*
 * A086947  Numbers k such that R(k+9) = 3.
 * https://oeis.org/A086947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3*(10^n - 3)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(n) is defined only for n >= 1 */
    if (n < 1)
        return (seq_t)0;

    seq_t pow10 = 1;
    /* Compute 10^n by repeated multiplication */
    for (idx_t i = 0; i < n; ++i)
        pow10 *= (seq_t)10;

    /* a(n) = 3 * (pow10 - 3) */
    pow10 -= (seq_t)3;
    pow10 *= (seq_t)3;
    return pow10;
}
