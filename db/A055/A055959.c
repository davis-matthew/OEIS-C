/*
 * A055959  n - reversal of base 9 digits of n (written in base 10).
 * https://oeis.org/A055959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = n - rev9(n), where rev9(n) is the number obtained by
 *            reversing the base‑9 digits of n and interpreting the reversed
 *            string again as a base‑9 number
 * Range:     i8 n <= 83, i16 n <= 84, i32 n <= 84, i64 n <= 84, i128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rev = 0;            /* result of reversed base‑9 conversion   */
    idx_t temp = n;           /* use idx_t for loop control             */
    while (temp > 0) {
        seq_t digit = (seq_t)(temp % 9);
        rev = rev * 9 + digit;
        temp /= 9;
    }
    return (seq_t)n - rev;
}
