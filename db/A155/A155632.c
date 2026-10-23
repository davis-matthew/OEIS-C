/*
 * A155632  a(n) = 11^n - 4^n + 1^n.
 * https://oeis.org/A155632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - 4^n + 1^n. Recurrence: a(0) = 1, a(1) = 8, a(n) =
 *            15*a(n-1) - 44*a(n-2) + 30 for n >= 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)8;   /* a(1) */
    seq_t current;

    for (idx_t i = 2; i <= n; ++i)
    {
        current = (seq_t)15 * prev1 - (seq_t)44 * prev2 + (seq_t)30;
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}
