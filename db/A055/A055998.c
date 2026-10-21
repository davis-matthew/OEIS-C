/*
 * A055998  a(n) = n*(n+5)/2.
 * https://oeis.org/A055998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+5)/2
 * Range:     u8 n <= 20, u16 n <= 359, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;            /* cast index to seq_t type */
    seq_t result = nn * (nn + 5) / 2;  /* compute the formula with integer arithmetic */
    return result;
}
