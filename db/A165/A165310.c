/*
 * A165310  a(0)=1, a(1)=3, a(n) = 7*a(n-1) - 9*a(n-2) for n > 1.
 * https://oeis.org/A165310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 3, a(n) = 7*a(n-1) - 9*a(n-2) (n > 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two initial values directly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;

    /* Iteratively build up to the requested index.  Only the two
       most recent terms need to be stored, which keeps the algorithm
       O(n) in time and O(1) in memory. */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)3;   /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 7*a(i-1) - 9*a(i-2) */
        curr = ( ( (seq_t)7 * prev1 ) - ( (seq_t)9 * prev2 ) );
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
