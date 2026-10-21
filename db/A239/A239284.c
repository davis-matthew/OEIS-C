/*
 * A239284  a(n) = (15^n - (-1)^n)/16.
 * https://oeis.org/A239284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (15^n - (-1)^n)/16 a(0) = 0, a(1) = 1, a(n) = 14*a(n-1) +
 *            15*a(n-2) for n > 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0;    /* a(0) */
    seq_t prev1 = (seq_t)1;    /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)14 * prev1 + (seq_t)15 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
