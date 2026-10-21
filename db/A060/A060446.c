/*
 * A060446  Number of ways to color vertices of a pentagon using <= n colors,
 *          allowing rotations and reflections.
 * https://oeis.org/A060446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^5 + 5*n^3 + 4*n)/10
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 86, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;          /* n */
    seq_t N2 = N * N;             /* n^2 */
    seq_t N3 = N2 * N;            /* n^3 */
    seq_t N5 = N3 * N2;           /* n^5 (n^3 * n^2) */

    seq_t poly = N5 + (seq_t)5 * N3 + (seq_t)4 * N;  /* n^5 + 5n^3 + 4n */
    return poly / (seq_t)10;       /* division is exact for all unsigned n */
}
