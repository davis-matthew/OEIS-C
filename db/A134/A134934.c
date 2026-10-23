/*
 * A134934  a(n) = (14*n+1)^2.
 * https://oeis.org/A134934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*n+1)^2
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 4681, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to sequence type */
    seq_t k = (seq_t)n;
    /* compute (14*k + 1) */
    seq_t t = 14 * k + 1;
    /* square the result */
    return t * t;
}
