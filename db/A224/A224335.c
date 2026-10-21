/*
 * A224335  Number of idempotent 4 X 4 0..n matrices of rank 3.
 * https://oeis.org/A224335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^3 + 24*n^2 + 24*n + 4
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;    /* n^2 */
    seq_t nn3 = nn2 * nn;   /* n^3 */

    seq_t result = nn3 * (seq_t)8 + nn2 * (seq_t)24 + nn * (seq_t)24 + (seq_t)4;
    return result;
}
