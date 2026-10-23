/*
 * A168401  a(n) = 4 + 9*floor(n/2).
 * https://oeis.org/A168401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4 + 9*floor(n/2)
 * (Offset 1: a(1) = 4)
 */
seq_t a(idx_t n)
{
    /* floor(n/2) can be obtained with a right shift. */
    seq_t k = (seq_t)n;
    seq_t f = k >> 1;              /* floor of n/2 */
    return (seq_t)4 + (seq_t)9 * f;
}
