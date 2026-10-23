/*
 * A176734  a(n) = (n+7)*a(n-1) + (n-1)*a(n-2), a(-1)=0, a(0)=1.
 * https://oeis.org/A176734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+7)*a(n-1) + (n-1)*a(n-2), a(-1)=0, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two values directly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)8;

    /* prev2 = a(n-2) and prev1 = a(n-1) for the current iteration. */
    seq_t prev2 = (seq_t)1;   /* a(0)   */
    seq_t prev1 = (seq_t)8;   /* a(1)   */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute a(i) = (i+7)*prev1 + (i-1)*prev2. */
        curr = ((seq_t)(i + 7) * prev1) + ((seq_t)(i - 1) * prev2);
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
