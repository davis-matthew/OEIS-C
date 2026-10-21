/*
 * A215537  Lowest k such that k is representable as both the sum of n and of
 *          n+1 nonzero squares.
 * https://oeis.org/A215537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + 9 for n >= 3; a(1)=25; a(2)=17
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 25;
    if (n == 2) return 17;
    return (seq_t)n + 9;
}
