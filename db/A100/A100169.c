/*
 * A100169  Structured pentagonal hexacontahedral numbers (vertex structure
 *          16).
 * https://oeis.org/A100169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (55*n^3 - 75*n^2 + 22*n)/2
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    seq_t term1 = (seq_t)55 * nn * nn * nn;
    seq_t term2 = (seq_t)75 * nn * nn;
    seq_t term3 = (seq_t)22 * nn;
    return (term1 - term2 + term3) / 2;
}
