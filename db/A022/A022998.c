/*
 * A022998  If n is odd then n, otherwise 2n.
 * https://oeis.org/A022998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n if n is odd, 2*n if n is even
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    if ((n & 1) == 0) {
        val *= (seq_t)2;
    }
    return val;
}
