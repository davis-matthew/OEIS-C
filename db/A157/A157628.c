/*
 * A157628  a(n) = 80000n^2 - 120800n + 45601.
 * https://oeis.org/A157628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 80000*n^2 - 120800*n + 45601
 * Range:     u8 none, u16 n <= 1, u32 n <= 232, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t temp = 80000 * nn;
    temp = temp * nn;          /* temp = 80000 * n^2 */
    temp = temp - (120800 * nn);
    temp = temp + 45601;
    return temp;
}
