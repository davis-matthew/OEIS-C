/*
 * A048727  a(n) = Xmult(n,7) or rule150(n,1).
 * https://oeis.org/A048727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = n XOR (n << 1) XOR (n << 2)
 * Range:     u8 n <= 63, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the binary encoding of n · (x^2 + x + 1) in GF(2)[X]. */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    return x ^ (x << 1) ^ (x << 2);
}
