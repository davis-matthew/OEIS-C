/*
 * A216972  a(4n+2) = 2, otherwise a(n) = n.
 * https://oeis.org/A216972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4n+2)=2, otherwise a(n)=n
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((n & 3ULL) == 2ULL) ? (seq_t)2 : (seq_t)n;
}
