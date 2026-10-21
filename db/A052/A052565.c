/*
 * A052565  E.g.f. (1+x^3-x^4)/(1-x).
 * https://oeis.org/A052565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! for n>3, with a(0)=1, a(1)=1, a(2)=2, a(3)=12
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the small special cases first */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)12;

    /* for n >= 4 compute n! directly */
    seq_t res = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        res *= (seq_t)i;
    return res;
}
