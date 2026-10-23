/*
 * A101442  a(n) = 9973*n + 10007.
 * https://oeis.org/A101442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 9973*n + 10007
 * Range:     u8 none, u16 n <= 5, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)9973 * (seq_t)n + (seq_t)10007;
}
