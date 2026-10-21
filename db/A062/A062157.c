/*
 * A062157  a(n) = 0^n - (-1)^n.
 * https://oeis.org/A062157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, mult
 * Formula:   a(n) = -(-1)^n
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    return (n & 1) ? (seq_t)1 : (seq_t)-1;
}
