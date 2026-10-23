/*
 * A132644  X-values of solutions to the equation X*(X + 1) - 13*Y^2 = 0.
 * https://oeis.org/A132644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1298*a(n-1) - a(n-2) + 648, a(0) = 0, a(1) = 324
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)324;

    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)324; /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = ((seq_t)1298) * prev1 - prev2 + ((seq_t)648);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
