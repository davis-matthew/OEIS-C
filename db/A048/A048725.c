/*
 * A048725  a(n) = Xmult(n,5) or rule90(n,1).
 * https://oeis.org/A048725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = n XOR (4n)
 * Range:     u8 n <= 63, u16 n <= 9336, u32 n <= 9336, u64 n <= 9336,
 *            u128 n <= 9336, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;      /* cast index to the correct value type */
    return x ^ (x << 2);     /* compute n ^ (4*n) */
}
