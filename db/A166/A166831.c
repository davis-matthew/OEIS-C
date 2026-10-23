/*
 * A166831  Number of nX3 1..3 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          nonincreasing order.
 * https://oeis.org/A166831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/90)*(n^6 + 9*n^5 + 55*n^4 + 150*n^3 + 124*n^2 - 339*n
 *            + 90)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;
    seq_t n6 = n5 * nn;
    seq_t num = n6
                + (seq_t)9   * n5
                + (seq_t)55  * n4
                + (seq_t)150 * n3
                + (seq_t)124 * n2
                - (seq_t)339 * nn
                + (seq_t)90;
    return num / (seq_t)90;
}
