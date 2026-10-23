/*
 * A166842  Number of n X 3 1..3 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          decreasing order.
 * https://oeis.org/A166842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (1/90)*n*(n^5 + 9*n^4 + 40*n^3 + 30*n^2 + 4*n + 6)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n2 * n2;
    seq_t n5 = n4 * nn;
    seq_t sum = n5 + 9 * n4 + 40 * n3 + 30 * n2 + 4 * nn + 6;
    seq_t prod = nn * sum;
    return prod / 90;
}
