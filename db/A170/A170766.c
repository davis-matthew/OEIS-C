/*
 * A170766  Expansion of g.f.: (1+x)/(1-46*x).
 * https://oeis.org/A170766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0, a(n)=47*46^(n-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t result = (seq_t)47;
    for (idx_t i = 1; i < n; ++i)
        result *= (seq_t)46;
    return result;
}
