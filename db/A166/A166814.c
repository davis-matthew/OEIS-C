/*
 * A166814  Number of nX2 1..3 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          increasing order.
 * https://oeis.org/A166814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 8*n^3 + 5*n^2 - 14*n)/12
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */
    seq_t numerator = n4
                     + (seq_t)8 * n3
                     + (seq_t)5 * n2
                     - (seq_t)14 * nn;
    return numerator / 12;
}
