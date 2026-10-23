/*
 * A008619  Positive integers repeated.
 * https://oeis.org/A008619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor(n/2) + 1 (equivalently (2*n + 3 + (-1)^n)/4)
 * Range:     u8 n <= 501, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n / 2) + 1;
}
