/*
 * A170738  Expansion of g.f.: (1+x)/(1-18*x).
 * https://oeis.org/A170738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0: a(n)=19*18^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                          /* a(0) = 1 */

    seq_t res = 19;                       /* a(1) = 19 */
    for (idx_t i = 2; i <= n; ++i)        /* multiply by 18 (n-1) times */
        res *= (seq_t)18;

    return res;                           /* a(n) = 19*18^(n-1) */
}
