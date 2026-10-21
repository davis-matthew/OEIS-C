/*
 * A097921  G.f.: (1-x^6)*(1-x^8)/((1-x)*(1-x^2)*(1-x^3)^2*(1-x^4)).
 * https://oeis.org/A097921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = { 1, n=0 or 1 ; 2 + floor(n*(n-1)/3), n>=2 }
 * Range:     u8 n <= 28, u16 n <= 443, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;
    }
    seq_t nn = (seq_t)n;
    return (seq_t)2 + (nn * (nn - 1)) / (seq_t)3;
}
