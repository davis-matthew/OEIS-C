/*
 * A173317  a(n) = 5*n! - 1.
 * https://oeis.org/A173317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n! - 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)4;
    }
    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;
    }
    return fact * (seq_t)5 - (seq_t)1;
}
