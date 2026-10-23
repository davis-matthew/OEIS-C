/*
 * A020876  a(n) = ((5+sqrt(5))/2)^n + ((5-sqrt(5))/2)^n.
 * https://oeis.org/A020876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2), a(0)=2, a(1)=5
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)5;

    seq_t prev2 = (seq_t)2;
    seq_t prev1 = (seq_t)5;
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = ((seq_t)5 * prev1) - ((seq_t)5 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
