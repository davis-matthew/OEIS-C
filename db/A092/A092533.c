/*
 * A092533  G.f.: (1+x^8)/((1-x)*(1-x^4)).
 * https://oeis.org/A092533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n < 4 * a(n) = 2*floor(n/4) for n ≥ 4
 * Range:     u8 n <= 298, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)1;
    else
        return (seq_t)2 * ((seq_t)n / 4);
}
