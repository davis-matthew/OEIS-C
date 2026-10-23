/*
 * A008768  Expansion of (1+x^7)/((1-x)*(1-x^2)*(1-x^3)*(1-x^4)).
 * https://oeis.org/A008768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2*n^3 + 9*n^2 + 72*n + 160)/144)
 * Range:     u8 n <= 24, u16 n <= 166, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 n128 = n;
    unsigned __int128 numerator = 2 * n128 * n128 * n128;
    numerator += 9 * n128 * n128;
    numerator += 72 * n128;
    numerator += 160;
    seq_t result = (seq_t)(numerator / 144);
    return result;
}
