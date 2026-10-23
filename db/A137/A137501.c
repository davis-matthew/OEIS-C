/*
 * A137501  The even numbers repeated, with alternating signs.
 * https://oeis.org/A137501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n if n even, else 1-n
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n;
    if ((n & 1) == 0)
        return res;
    return (seq_t)(1 - res);
}
