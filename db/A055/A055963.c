/*
 * A055963  n - reversal of base 12 digits of n (written in base 10).
 * https://oeis.org/A055963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = n - rev_base12(n) where rev_base12(n) is the number
 *            obtained by writing n in base‑12, reversing its digits, and
 *            interpreting the result as a base‑12 number
 * Range:     i8 n <= 143, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rev = 0;

    /* Build the reversed base‑12 number. */
    for (idx_t t = n; t != 0; t /= 12)
    {
        seq_t digit = t % 12;   /* current least‑significant base‑12 digit */
        rev = rev * 12 + digit;
    }

    /* Return n minus the reversed base‑12 value. */
    return (seq_t)n - rev;
}
