/*
 * A102285  G.f. (1-x)/(7*x^2-6*x+1).
 * https://oeis.org/A102285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) for n > 1; a(0)=1, a(1)=5
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)5;
    }
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)5;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = 6 * prev1 - 7 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
