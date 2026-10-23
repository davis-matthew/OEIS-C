/*
 * A157915  a(n) = 625*n^2 + 25.
 * https://oeis.org/A157915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 625*n^2 + 25
 * Range:     u8 none, u16 n <= 10, u32 n <= 2621, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* convert index to seq_t */
    seq_t val = sn * sn;          /* n^2 */
    val *= 625;                   /* 625 * n^2 */
    val += 25;                    /* + 25 */
    return val;
}
