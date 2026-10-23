/*
 * A183615  1/120 the number of (n+1) X 3 0..4 arrays with every 2 X 2
 *          subblock containing four distinct values.
 * https://oeis.org/A183615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 24*a(n-2) for n >= 3, and the initial values
 *            a(1) = 6, a(2) = 56
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)56;

    seq_t prev2 = (seq_t)6;
    seq_t prev1 = (seq_t)56;
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i)
    {
        curr = ((seq_t)12 * prev1) - ((seq_t)24 * prev2);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
