/*
 * A048772  Partial sums of A048696.
 * https://oeis.org/A048772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 2*a(n-1)+a(n-2)+8; a(0) = 1, a(1) = 10
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)10;

    seq_t prev2 = (seq_t)1; /* a(0) */
    seq_t prev1 = (seq_t)10;/* a(1) */
    seq_t curr = 0;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)2 * prev1 + prev2 + (seq_t)8;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
