/*
 * A121990  Expansion of x*(1+9*x+2*x^2)/((1-x)*(1-3*x+x^2)).
 * https://oeis.org/A121990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + 12, with a(1)=1, a(2)=13
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)13;

    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)13;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t curr = 3 * prev1 - prev2 + (seq_t)12;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
