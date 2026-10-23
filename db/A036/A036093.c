/*
 * A036093  Centered cube numbers: (n+1)^15 + n^15.
 * https://oeis.org/A036093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^15 + (n+1)^15
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 17, u128 n <= 353,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t y = x + 1U;
    seq_t pow_x = 1;
    seq_t pow_y = 1;
    /* compute 15-th powers iteratively */
    for (int i = 0; i < 15; ++i) {
        pow_x *= x;
        pow_y *= y;
    }
    return pow_x + pow_y;
}
