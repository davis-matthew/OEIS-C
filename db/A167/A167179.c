/*
 * A167179  The number of additional armies one receives in Parker Brothers'
 *          (now part of Hasbro) game of Risk for turning in the n-th set of
 *          three different or alike cards.
 * https://oeis.org/A167179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2*(n+1) for n <= 5, otherwise a(n) = 5*(n-3)
 * Range:     u8 n <= 54, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 5)
        return (seq_t)n * 2 + 2;
    else
        return (seq_t)n * 5 - 15;
}
