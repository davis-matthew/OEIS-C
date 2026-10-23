/*
 * A165394  Number of slanted 2 X n (i=1..2) X (j=i..n+i-1) 1..4 arrays with
 *          all 1s connected, all 2s connected, all 3s connected, all 4s
 *          connected, 1 in the upper left corner, 2 in the upper right
 *          corner, 3 in the lower left corner, 4 in the lower right corner,
 *          and with no element having more than 3 neighbo...
 * https://oeis.org/A165394
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 - 4*n^3 + 11*n^2 - 20*n + 15) / 3 (n ≥ 2)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* outside the offset; behaviour not specified */

    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;
    seq_t n3   = n2 * nn;
    seq_t n4   = n3 * nn;

    seq_t num = n4 - 4 * n3 + 11 * n2 - 20 * nn + 15;
    return num / 3;
}
