/*
 * A166114  a(n) = (6-(-4)^n)/5.
 * https://oeis.org/A166114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 6 - 4*a(n-1), a(0) = 1
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 32, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alternately a(n) = (6-(-4)^n)/5,         a(0)=1  */
seq_t a(idx_t n)
{
    seq_t value = 1;          /* a(0) */
    if (n == 0) return value;
    for (idx_t i = 1; i <= n; ++i) {
        value = (seq_t)6 - (seq_t)4 * value;
    }
    return value;
}
