/*
 * A139746  a(n) = 11^n - 8^n.
 * https://oeis.org/A139746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - 8^n Recurrence: a(0) = 0, a(1) = 3, a(n) =
 *            19*a(n-1) - 88*a(n-2) for n >= 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)0;
    if (n == 1ULL) return (seq_t)3;

    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)3;   /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)19 * prev1 - (seq_t)88 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
