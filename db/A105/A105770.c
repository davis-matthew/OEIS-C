/*
 * A105770  Expansion of (x^2-x+1)*(4*x^2+x+1) / ((1+x+x^2)*(1-x)^3).
 * https://oeis.org/A105770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 + 1 + (n mod 3 == 2 ? 2 : 0)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;
    seq_t res = sq + 1;
    if (n % 3 == 2)
        res += 2;
    return res;
}
