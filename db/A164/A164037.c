/*
 * A164037  Expansion of (5-9*x)/(1-6*x+7*x^2).
 * https://oeis.org/A164037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2); a(0)=5, a(1)=21
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)21;

    seq_t prev2 = (seq_t)5;     /* a(0) */
    seq_t prev1 = (seq_t)21;    /* a(1) */
    seq_t curr  = (seq_t)0;     /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)6 * prev1 - (seq_t)7 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
