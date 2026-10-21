/*
 * A055967  n - reversal of base 20 digits of n (written in base 10).
 * https://oeis.org/A055967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = n - rev20(n) where rev20(n) is the base‑20 digit reversal
 *            of n
 * Range:     i8 n <= 27, i16 n <= 68, i32 n <= 68, i64 n <= 68, i128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rev = 0;
    idx_t t = n;
    while (t > 0)
    {
        idx_t d = t % 20;
        rev = rev * 20 + (seq_t)d;
        t /= 20;
    }
    return (seq_t)n - rev;
}
