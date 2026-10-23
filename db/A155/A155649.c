/*
 * A155649  a(n) = 11^n + 6^n - 1^n.
 * https://oeis.org/A155649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n + 6^n - 1^n Recurrence: a(0) = 1, a(1) = 16, a(n) =
 *            17*a(n-1) - 66*a(n-2) - 50 (n >= 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)16;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)16;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * (seq_t)17 - prev2 * (seq_t)66 - (seq_t)50;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
