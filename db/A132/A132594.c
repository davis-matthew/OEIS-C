/*
 * A132594  Values X satisfying the equation: X(X + 1) - 7*Y^2 = 0.
 * https://oeis.org/A132594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 254*a(n-1) - a(n-2) + 126 with a(0) = 0, a(1) = 63
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 63;

    seq_t prev2 = 0;          /* a(n-2) */
    seq_t prev1 = 63;         /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 254 * prev1 - prev2 + 126;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
