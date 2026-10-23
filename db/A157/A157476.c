/*
 * A157476  a(n) = 2048n^2 + 128n + 1.
 * https://oeis.org/A157476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2048*n^2 + 128*n + 1
 * Range:     u8 none, u16 n <= 5, u32 n <= 1448, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    /* compute 2048*n^2  as n << 11 * n  */
    seq_t term1 = t * (t << 11);   /* 2048 * n^2   */
    seq_t term2 = t << 7;          /* 128  * n     */
    return term1 + term2 + (seq_t)1;
}
