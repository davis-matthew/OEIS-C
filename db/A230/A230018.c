/*
 * A230018  a(n) = (9*n^3 + 5*n)/2.
 * https://oeis.org/A230018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^3 + 5*n)/2
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t n3 = N * N * N;            /* n³ */
    seq_t num = (seq_t)9 * n3 + (seq_t)5 * N;  /* 9*n³ + 5*n */
    return num / (seq_t)2;            /* divide by 2 */
}
