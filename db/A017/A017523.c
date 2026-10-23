/*
 * A017523  a(n) = (12*n)^3.
 * https://oeis.org/A017523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12·n)^3
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 135, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;      /* cast index to seq_t first */
    val = val * val * val * 1728; /* 12^3 = 1728, compute n³·1728 */
    return val;
}
