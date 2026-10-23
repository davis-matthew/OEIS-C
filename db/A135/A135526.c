/*
 * A135526  Number of sums payable using exactly n banknotes of denominations
 *          1, 5, 10, 20, 50, 100 (change allowable).
 * https://oeis.org/A135526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*n - 212 for n>=4, with a(0)=1, a(1)=6, a(2)=33,
 *            a(3)=95
 * Range:     u8 n <= 4, u16 n <= 657, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 6;
    if (n == 2) return 33;
    if (n == 3) return 95;
    return (seq_t)100 * n - 212;
}
