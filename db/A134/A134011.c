/*
 * A134011  Period 9: repeat [1, 2, 3, 4, 5, 4, 3, 2, 1].
 * https://oeis.org/A134011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-9) for n>8, with initial values a(0..8) = [1, 2, 3,
 *            4, 5, 4, 3, 2, 1]
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[9] = {1, 2, 3, 4, 5, 4, 3, 2, 1};
    return period[n % 9];
}
