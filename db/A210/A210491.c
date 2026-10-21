/*
 * A210491  Planck temperature T_P.
 * https://oeis.org/A210491
 *
 * Offset:    33    (a(33) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = the (n-33)-th decimal digit of the integer 1416833 ×
 *            10^26 (the decimal expansion of Planck temperature 1.416833 ×
 *            10^32)
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 33) return (seq_t)0;          /* Undefined for indices < 33 */
    static const char digits[7] = {'1','4','1','6','8','3','3'};
    idx_t pos = n - 33;
    if (pos < 7) return (seq_t)(digits[pos] - '0');
    return (seq_t)0;                      /* All further digits are zero */
}
