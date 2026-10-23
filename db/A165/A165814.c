/*
 * A165814  a(n)=n*(a(n-1)-3), a(0)=1.
 * https://oeis.org/A165814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n*(a(n-1)-3), a(0) = 1 (OEIS recurrence for sequence
 *            A165814) Computes a(n) exactly by iterating the recurrence from
 *            0 to n
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 19, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        result = (seq_t)i * (result - (seq_t)3);
    }
    return result;
}
