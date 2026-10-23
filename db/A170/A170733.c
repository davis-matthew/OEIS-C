/*
 * A170733  Expansion of g.f.: (1+x)/(1-13*x).
 * https://oeis.org/A170733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0, a(1)=14, a(n)=13*a(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */
    seq_t res = (seq_t)14;        /* a(1) = 14 */
    if (n == 1)
        return res;               /* return if n == 1 */

    /* compute 13^(n-1) * 14 iteratively */
    idx_t i = n - 1;
    while (i > 0) {
        res *= (seq_t)13;
        --i;
    }
    return res;
}
