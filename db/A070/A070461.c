/*
 * A070461  a(n) = n^2 mod 39.
 * https://oeis.org/A070461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 mod 39
 * Range:     u8 n <= 15, u16 n <= 328, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * (seq_t)n;
    return m % (seq_t)39;
}
