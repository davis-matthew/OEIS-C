/*
 * A184145  1/4 the number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock
 *          having at least two equal elements connected horizontally or
 *          vertically.
 * https://oeis.org/A184145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) + 28*a(n-2) with initial values a(1) = 43, a(2)
 *            = 499
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial cases directly. */
    if (n == 0) return 0;          /* not part of the sequence, safety */
    if (n == 1) return (seq_t)43;
    if (n == 2) return (seq_t)499;

    /* Two previous terms: a(n-2) and a(n-1). */
    seq_t prev2 = (seq_t)43;    /* a(1) */
    seq_t prev1 = (seq_t)499;   /* a(2) */
    seq_t cur = 0;

    /* Iterate from i = 3 up to n using the recurrence. */
    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)9 * prev1 + (seq_t)28 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
