/*
 * A173511  a(n) = 4*n^2 + floor(n/2).
 * https://oeis.org/A173511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2 + floor(n/2)
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t once */
    seq_t tn = (seq_t)n;

    /* Compute 4*n^2 */
    seq_t val = tn * tn;      /* n^2   */
    val *= 4;                 /* 4*n^2 */

    /* Add floor(n/2) */
    val += tn / 2;

    return val;
}
