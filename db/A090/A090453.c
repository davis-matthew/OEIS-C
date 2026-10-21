/*
 * A090453  Third column (m=4) of array A090452.
 * https://oeis.org/A090453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n^3+15*n^2+56*n+24)/12
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 277, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;               /* promote index to seq_t */
    seq_t N2 = N * N;                 /* n^2 */
    seq_t N3 = N2 * N;                /* n^3 */
    seq_t poly = N3 + (seq_t)15 * N2 + (seq_t)56 * N + (seq_t)24;
    seq_t num = (N + (seq_t)1) * poly;
    return num / (seq_t)12;
}
