/*
 * A157364  a(n) = 4802*n^2 - 196*n + 1.
 * https://oeis.org/A157364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4802*n^2 - 196*n + 1
 * Range:     u8 none, u16 n <= 3, u32 n <= 945, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the result type */
    seq_t N = (seq_t)n;

    /* Compute N^2 */
    seq_t nn = N * N;

    /* Compute 4802 * N^2 - 196 * N + 1 */
    seq_t res = 4802 * nn;
    res -= 196 * N;
    res += 1;

    return res;
}
