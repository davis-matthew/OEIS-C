/*
 * A072277  Smallest integer > 1 which is both n-gonal and centered n-gonal.
 * https://oeis.org/A072277
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n^2 + 2)/2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;
    return (n3 - n2 + 2) / 2;
}
