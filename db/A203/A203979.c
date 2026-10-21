/*
 * A203979  Number of (n+1)X4 0..2 arrays with no 2X2 subblock having equal
 *          diagonal elements or equal antidiagonal elements, and new values
 *          0..2 introduced in row major order.
 * https://oeis.org/A203979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 96*9^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1U) return (seq_t)0;
    seq_t res = (seq_t)96;
    for (idx_t i = 1U; i < n; ++i)
        res *= (seq_t)9;
    return res;
}
