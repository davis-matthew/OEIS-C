/*
 * A180222  a(n) = 4*a(n-1) + 8*a(n-2), with a(1)=0 and a(2)=1.
 * https://oeis.org/A180222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 8*a(n-2), a(1)=0, a(2)=1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly */
    if (n <= 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)1;

    /* Iterate from 3 up to n using only two previous values */
    seq_t prev2 = (seq_t)0;   /* a(1) */
    seq_t prev1 = (seq_t)1;   /* a(2) */
    seq_t cur   = 0;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)4 * prev1 + (seq_t)8 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;               /* cur holds a(n) after the loop */
}
