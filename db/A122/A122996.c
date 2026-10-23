/*
 * A122996  Expansion of (1+6*x)/(1-x-49*x^2).
 * https://oeis.org/A122996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 49*a(n-2), a(0)=1, a(1)=7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    seq_t prev2 = (seq_t)1; /* a(0) */
    seq_t prev1 = (seq_t)7; /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 + prev2 * (seq_t)49;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
