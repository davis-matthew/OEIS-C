/*
 * A055965  n - reversal of hexadecimal (base 16) digits of n (written in base
 *          10).
 * https://oeis.org/A055965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = n - A056962(n) // A056962(n) = reversal of hex digits of
 *            n
 * Range:     i8 n <= 25, i16 n <= 68, i32 n <= 68, i64 n <= 68, i128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t rev = 0;
    seq_t t = N;
    /* Reverse hex digits by repeated division by 16 */
    while (t != 0) {
        seq_t digit = t % (seq_t)16;
        rev = rev * (seq_t)16 + digit;
        t = t / (seq_t)16;
    }
    return N - rev;
}
