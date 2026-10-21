/*
 * A052800  E.g.f.: x^5*exp(x)-x^5.
 * https://oeis.org/A052800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n < 6; a(n) = n*(n-1)*(n-2)*(n-3)*(n-4) for n >= 6
 *            (equivalently a(n)=n!/(n-5)!)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 86, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 6)
        return (seq_t)0;

    seq_t res = 1;
    for (idx_t i = 0; i < 5; ++i)
        res *= (seq_t)(n - i);

    return res;
}
