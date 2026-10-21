/*
 * A055953  n - reversal of base 6 digits of n (written in base 10).
 * https://oeis.org/A055953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = n - A030105(n) (A030105(n) is the integer obtained by
 *            reversing the base‑6 digits of n and interpreting the resulting
 *            string as a base‑6 number)
 * Range:     i8 n <= 40, i16 n <= 7792, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rev = 0;        /* reversed base‑6 number (in decimal) */
    seq_t temp = (seq_t)n;

    while (temp > 0)
    {
        int digit = (int)(temp % 6);   /* least significant base‑6 digit */
        rev = rev * 6 + digit;         /* build reversed number */
        temp = temp / 6;               /* shift right in base‑6 */
    }

    return (seq_t)n - rev;               /* a(n) = n - rev */
}
