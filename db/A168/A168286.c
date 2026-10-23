/*
 * A168286  a(n) = (6*n + 3*(-1)^n + 1)/2.
 * https://oeis.org/A168286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (6*n + 3*(-1)^n + 1)/2 (offset 1)
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;              /* convert index to seq_t once */
    if ((n & 1) == 0)                /* n even  →  a(n) = 3n + 2 */
        return 3 * sn + 2;
    else                             /* n odd   →  a(n) = 3n - 1 */
        return 3 * sn - 1;
}
