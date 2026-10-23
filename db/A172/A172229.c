/*
 * A172229  Number of ways to place 3 nonattacking wazirs on a 3 X n board.
 * https://oeis.org/A172229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n - 5)*(3*n^2 - 8*n + 8)/2 for n>=2, a(1) = 0
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 812, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    seq_t nn  = (seq_t)n;
    seq_t t1  = 3*nn - 5;
    seq_t t2  = 3*nn*nn - 8*nn + 8;
    return (t1 * t2) / 2;
}
