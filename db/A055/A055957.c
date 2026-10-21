/*
 * A055957  n - reversal of base 8 digits of n (written in base 10).
 * https://oeis.org/A055957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = n - rev8(n) where rev8(n) is n with its base‑8 digits
 *            reversed, interpreted as a decimal number
 * Range:     i8 n <= 67, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t orig = n;          /* keep original value as it will be destroyed */
    unsigned __int128 rev = 0; /* use 128‑bit to avoid overflow of the reverse */
    idx_t tmp = orig;

    while (tmp) {
        rev = rev * 8 + (tmp & 7); /* append least significant octal digit */
        tmp >>= 3;                 /* divide by 8 */
    }

    return (seq_t)orig - (seq_t)rev;
}
