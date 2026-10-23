/*
 * A158491  a(n) = 20*n^2 - 1.
 * https://oeis.org/A158491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*n^2 - 1
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid overflow on intermediate results
       (for fixed-width types this is safe as 20*n^2 fits easily for the
       intended range; for arbitrary-precision types the multiplication
       will promote to the needed size). */
    seq_t sn = (seq_t)n;
    seq_t val = sn * sn;   /* n^2 */
    val *= 20;             /* 20*n^2 */
    val -= 1;              /* 20*n^2 - 1 */
    return val;
}
