/*
 * A226023  A142705 (numerators of 1/4-1/(4n^2)) sorted to natural order.
 * https://oeis.org/A226023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n+1)/3) * floor((2*n+5)/3)
 * Range:     u8 n <= 23, u16 n <= 383, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t two_n = (seq_t)n * 2;
    seq_t left  = (two_n + 1) / 3;
    seq_t right = (two_n + 5) / 3;
    return left * right;
}
