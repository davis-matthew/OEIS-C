/*
 * A063249  Doubly hexagonal numbers.
 * https://oeis.org/A063249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n-1)*(4*n^2-2*n-1) = H(H(n)) where H(n)=n*(2*n-1)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 152, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n = 0 separately to avoid unsigned underflow */
    if (n == 0u) return (seq_t)0;

    seq_t nn = (seq_t)n;
    /* First hexagonal number: H(n) = n*(2*n-1) */
    seq_t h = nn * ((seq_t)2 * nn - (seq_t)1);
    /* Second hexagonal number: H(h) = h*(2*h-1) */
    seq_t two_h_minus1 = (seq_t)2 * h - (seq_t)1;
    return h * two_h_minus1;
}
