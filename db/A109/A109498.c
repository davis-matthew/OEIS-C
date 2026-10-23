/*
 * A109498  Number of closed walks of length 2n on the Heawood graph from a
 *          given node.
 * https://oeis.org/A109498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 18*a(n-2), a(0) = 1, a(1) = 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Use the recurrence to build up to n. */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)3;   /* a(1) */
    seq_t cur   = 0;          /* temporary holder for a(i) */

    for (idx_t i = 2; i <= n; ++i) {
        cur   = (seq_t)11 * prev1 - (seq_t)18 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
