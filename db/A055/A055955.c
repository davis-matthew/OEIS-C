/*
 * A055955  a(n) = n - reversal of base 7 digits of n (written in base 10).
 * https://oeis.org/A055955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = n - reversal_of_base7_digits(n)
 * Range:     i8 n <= 52, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t tmp = n;
    idx_t rev = 0;
    while (tmp > 0) {
        rev = rev * 7 + (tmp % 7);
        tmp /= 7;
    }
    return (seq_t)n - (seq_t)rev;
}
