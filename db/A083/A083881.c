/*
 * A083881  a(n) = 6*a(n-1) - 6*a(n-2), with a(0)=1, a(1)=3.
 * https://oeis.org/A083881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 3, a(n) = 6*a(n-1) - 6*a(n-2) for n >= 2 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)3;   /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = ((seq_t)6 * prev1) - ((seq_t)6 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
