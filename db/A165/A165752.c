/*
 * A165752  a(n) = (8-5*4^n)/3.
 * https://oeis.org/A165752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (8-5*4^n)/3
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(n)=4*a(n-1)-8  (n≥1) */

seq_t a(idx_t n)
{
    seq_t val = 1;                      /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        val = val * (seq_t)4 - (seq_t)8;/* a(i) = 4*a(i-1) - 8   */
    }
    return val;
}
