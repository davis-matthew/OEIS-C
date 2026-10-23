/*
 * A016243  Expansion of 1/((1-x)*(1-6*x)*(1-8*x)).
 * https://oeis.org/A016243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 48*a(n-2) + 1 (a(0)=1, a(1)=15)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
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
        return (seq_t)15;

    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)15; /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = (seq_t)14 * prev1 - (seq_t)48 * prev2 + (seq_t)1;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
