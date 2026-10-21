/*
 * A070534  a(n) = n^4 mod 17.
 * https://oeis.org/A070534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 17
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)(n % 17);
    seq_t m2 = (m * m) % (seq_t)17;
    return (m2 * m2) % (seq_t)17;
}
