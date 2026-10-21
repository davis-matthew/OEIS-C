/*
 * A239171  Number of (n+1) X (1+1) 0..2 arrays with no element greater than
 *          all horizontal neighbors or equal to all vertical neighbors.
 * https://oeis.org/A239171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2), a(1)=6, a(2)=12
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)6;
    if (n == 2)
        return (seq_t)12;

    seq_t prev2 = (seq_t)6;   /* a(1) */
    seq_t prev1 = (seq_t)12;  /* a(2) */
    seq_t cur = 0;

    for (idx_t i = 3; i <= n; ++i) {
        cur = ((seq_t)2 * prev1) + ((seq_t)2 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
