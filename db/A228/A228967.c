/*
 * A228967  Table read by rows; T(n,k) = 2*n for k = 1, T(n,k) = 9*n for k =
 *          2.
 * https://oeis.org/A228967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = 2*((n+1)/2) if n is odd, 9*((n+1)/2) if n is even
 * Range:     u8 n <= 57, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t half = ((seq_t)n + 1) / 2;
    if (n & 1)
        return 2 * half;
    else
        return 9 * half;
}
