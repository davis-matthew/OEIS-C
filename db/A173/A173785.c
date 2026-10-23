/*
 * A173785  Expansion of 2*(1 -4*x +14*x^2 +4*x^3 +9*x^4)/(1-x)^5.
 * https://oeis.org/A173785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n^2 - n + 1)^2
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 215, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t = nn * nn - nn + 1;
    return 2 * t * t;
}
