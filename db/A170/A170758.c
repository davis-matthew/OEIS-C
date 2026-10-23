/*
 * A170758  Expansion of g.f.: (1+x)/(1-38*x).
 * https://oeis.org/A170758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(n)=39*38^(n-1) for n>0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res = (seq_t)1;
    /* multiply 38 (n-1) times */
    for (idx_t i = 1; i < n; ++i)
        res *= (seq_t)38;

    /* final multiplication by 39 */
    res *= (seq_t)39;
    return res;
}
