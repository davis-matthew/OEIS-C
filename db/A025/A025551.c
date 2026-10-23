/*
 * A025551  a(n) = 3^n*(3^n + 1)/2.
 * https://oeis.org/A025551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n * (3^n + 1) / 2 Recurrence (derived from the OEIS
 *            entry): a(0) = 1, a(1) = 6 a(n) = 12 * a(n-1) - 27 * a(n-2) for
 *            n >= 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
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
        return (seq_t)6;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)6;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)12 * prev1 - (seq_t)27 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
