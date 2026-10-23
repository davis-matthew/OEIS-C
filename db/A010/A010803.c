/*
 * A010803  15th powers: a(n) = n^15.
 * https://oeis.org/A010803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^15
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 19, u128 n <= 370,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t x = (seq_t)n;
    seq_t r = 1;
    for (int i = 0; i < 15; ++i) r *= x;
    return r;
}
