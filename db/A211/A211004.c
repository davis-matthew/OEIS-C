/*
 * A211004  Number of distinct regions in the set of partitions of n.
 * https://oeis.org/A211004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = ∑_{j=1}^{n+2} ⌊j/3⌋
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = n+2 to account for sum limits */
    idx_t m = n + 2;
    /* Complete groups of size 3 */
    idx_t G = m / 3;
    /* Remaining terms after the last complete group */
    idx_t R = m - G * 3;

    /* tmp = 3*G*G - G  (fits in idx_t for all practical n) */
    idx_t tmp = 3 * G * G - G;

    /* a(n) = (tmp/2) + G*R  */
    seq_t result = (seq_t)(tmp / 2 + G * R);
    return result;
}
