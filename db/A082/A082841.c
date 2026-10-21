/*
 * A082841  a(n) = 4*a(n-1) - a(n-2) for n>1, a(0)=3, a(1)=9.
 * https://oeis.org/A082841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2), a(0)=3, a(1)=9
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial two terms explicitly */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)9;

    /* Iterate using two previous values */
    seq_t prev2 = (seq_t)3;  /* a(0) */
    seq_t prev1 = (seq_t)9;  /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)4 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
