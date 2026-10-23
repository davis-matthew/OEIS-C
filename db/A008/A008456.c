/*
 * A008456  12th powers: a(n) = n^12.
 * https://oeis.org/A008456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^12
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 40, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    seq_t res = (seq_t)1;
    /* perform 12 successive multiplications */
    for (int i = 0; i < 12; ++i)
        res *= t;
    return res;
}
