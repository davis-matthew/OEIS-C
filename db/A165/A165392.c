/*
 * A165392  Number of slanted 2 X n (i=1..2) X (j=i..n+i-1) 1..4 arrays with
 *          all 1s connected, all 2s connected, all 3s connected, all 4s
 *          connected, 1 in the upper left corner, 2 in the upper right
 *          corner, 3 in the lower left corner, 4 in the lower right corner,
 *          and with no element having more than 2 neighbo...
 * https://oeis.org/A165392
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n^2 - 7*n - 1 for n > 5 a(2) = 1, a(3) = 9, a(4) = 33,
 *            a(5) = 65 (initial terms)
 * Range:     u8 n <= 8, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;                       /* undefined for n < 2 */
    if (n == 2) return 1;
    if (n == 3) return 9;
    if (n == 4) return 33;
    if (n == 5) return 65;

    /* uses the closed‑form a(n) = 4*n^2 - 7*n - 1   for n > 5 */
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;             /* n^2, safe for n up to ~10^6 in 32‑bit */
    return 4 * sq - 7 * nn - 1;
}
