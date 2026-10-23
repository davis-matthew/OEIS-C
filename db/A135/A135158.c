/*
 * A135158  a(n) = 5^n - 3^n - 2^n.
 * https://oeis.org/A135158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 5^n - 3^n - 2^n
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 27, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow5 = (seq_t)1;
    seq_t pow3 = (seq_t)1;
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow5 *= (seq_t)5;
        pow3 *= (seq_t)3;
        pow2 *= (seq_t)2;
    }
    return pow5 - pow3 - pow2;
}
