/*
 * A155674  a(n) = 11^n - 10^n + 1^n.
 * https://oeis.org/A155674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - 10^n + 1^n Recurrence: a(0) = 1, a(1) = 2, a(n) =
 *            21*a(n-1) - 110*a(n-2) + 90 (valid for all n >= 0)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t prev2 = (seq_t)1;    /* a(0) */
    seq_t prev1 = (seq_t)2;    /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)21 * prev1 - (seq_t)110 * prev2 + (seq_t)90;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
