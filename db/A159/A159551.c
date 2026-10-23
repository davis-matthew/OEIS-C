/*
 * A159551  a(n) = 101*n + 10.
 * https://oeis.org/A159551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 101*n + 10
 * Range:     u8 n <= 2, u16 n <= 648, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the linear function 101*n + 10.
       All operations are performed in the seq_t type. */
    return ((seq_t)n) * 101 + 10;
}
