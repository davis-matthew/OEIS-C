/*
 * A123009  Expansion of x*(1 + 5*x)/(1 - 2*x - 49*x^2).
 * https://oeis.org/A123009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 49*a(n-2), a(1)=1, a(2)=7
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Return the first two terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;

    /* Use the recurrence to compute the n-th term */
    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)7;   /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur   = (seq_t)2 * prev1 + (seq_t)49 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
