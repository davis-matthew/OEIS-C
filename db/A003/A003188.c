/*
 * A003188  Decimal equivalent of Gray code for n.
 * https://oeis.org/A003188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, look
 * Formula:   a(n) = n XOR floor(n/2)
 * Range:     u8 n <= 255, u16 n <= 8019, u32 n <= 8019, u64 n <= 8019,
 *            u128 n <= 8019, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
	seq_t nval = (seq_t)n;
	return nval ^ (nval >> 1);
}
