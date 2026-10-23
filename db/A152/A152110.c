/*
 * A152110  G.f.: -2*(-2 - 11*x - 4*x^2 + x^3)/(x - 1)^4.
 * https://oeis.org/A152110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16*n^3 + 48*n^2 + 38*n + 12)/3
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 720, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = (seq_t)16 * nn * nn * nn
              + (seq_t)48 * nn * nn
              + (seq_t)38 * nn
              + (seq_t)12;
    return num / 3;
}
