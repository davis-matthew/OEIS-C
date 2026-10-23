/*
 * A165809  a(n) = 3*n*(310*n^2 + 63*n - 12).
 * https://oeis.org/A165809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(310*n^2 + 63*n - 12)
 * Range:     u8 none, u16 n <= 4, u32 n <= 166, u64 n <= 5000, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;                     /* convert index to seq_t */
    seq_t tmp = 310 * N * N + 63 * N - 12;
    return 3 * N * tmp;
}
