/*
 * A213667  Number of dominating subsets with k vertices in all the graphs
 *          G(n) (n>=1) obtained by taking n copies of the path P_3 and
 *          identifying one of their endpoints (a star with n branches of
 *          length 2).
 * https://oeis.org/A213667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2 for n >= 4 Initial values: a(1)=1,
 *            a(2)=6, a(3)=16
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)6;
    if (n == 3u) return (seq_t)16;

    seq_t prev2 = (seq_t)6;   /* a(2) */
    seq_t prev1 = (seq_t)16;  /* a(3) */
    seq_t curr   = (seq_t)0;

    for (idx_t i = 4; i <= n; ++i) {
        curr = (seq_t)2 * prev1 + prev2 + (seq_t)2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
