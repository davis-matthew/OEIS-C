/*
 * A022314  a(n) = a(n-1) + a(n-2) + 1, with a(0) = 0, a(1) = 9.
 * https://oeis.org/A022314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 1, a(0) = 0, a(1) = 9
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two initial values directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)9;

    /* Iteratively compute the recurrence up to n. */
    seq_t prev2 = (seq_t)0;   /* a(0)   */
    seq_t prev1 = (seq_t)9;   /* a(1)   */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = prev1 + prev2 + (seq_t)1;  /* a(i) = a(i-1)+a(i-2)+1 */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
