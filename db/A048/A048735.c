/*
 * A048735  a(n) = (n AND floor(n/2)), where AND is bitwise and-operator
 *          (A004198).
 * https://oeis.org/A048735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n & floor(n/2)
 * Range:     u8 n <= 767, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n) & ((seq_t)(n >> 1));
}
