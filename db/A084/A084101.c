/*
 * A084101  Expansion of (1+x)^2/((1-x)*(1+x^2)).
 * https://oeis.org/A084101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(3, n mod 4)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[4] = {1, 3, 3, 1};
    return tbl[n & 3];
}
