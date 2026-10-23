/*
 * A105397  Periodic with period 2: repeat [4,2].
 * https://oeis.org/A105397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4 - 2 * (n mod 2) (period 2: [4,2])
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is unsigned; (n & 1) gives n mod 2 */
    return (seq_t)4 - ((seq_t)2 * ((seq_t)n & (seq_t)1));
}
