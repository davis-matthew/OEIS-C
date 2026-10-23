/*
 * A157928  a(n) = 0 if n < 2, = 1 otherwise.
 * https://oeis.org/A157928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n < 2, otherwise 1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n >= 2) ? (seq_t)1 : (seq_t)0;
}
