/*
 * A175672  y-values in the solution to x^2 - 28*y^2=1.
 * https://oeis.org/A175672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 254*a(n-1) - a(n-2), a(1)=0, a(2)=24
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)24;
    seq_t prev2 = (seq_t)0;
    seq_t prev1 = (seq_t)24;
    idx_t i;
    for (i = 3; i <= n; ++i) {
        seq_t cur = 254 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
