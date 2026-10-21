/*
 * A090843  Number of nodes on a tree with degree 11 interior nodes and degree
 *          1 boundary nodes.
 * https://oeis.org/A090843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (11*10^n - 2)/9 = 10*a(n-1) + 2, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t value = (seq_t)1;          /* a(0) */
    if (n == 0)
        return value;

    for (idx_t i = 1; i <= n; ++i)
        value = value * (seq_t)10 + (seq_t)2;

    return value;
}
