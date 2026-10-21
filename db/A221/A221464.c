/*
 * A221464  Number of 0..n arrays of length 5 with each element unequal to at
 *          least one neighbor, starting with 0.
 * https://oeis.org/A221464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 + 2*n^3
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 137, u64 n <= 137,
 *            u128 n <= 137, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;
    seq_t n2 = x * x;            /* n² */
    seq_t n3 = n2 * x;           /* n³ */
    seq_t n4 = n3 * x;           /* n⁴ */
    return n4 + (seq_t)2 * n3;   /* n⁴ + 2·n³ */
}
