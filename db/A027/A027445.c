/*
 * A027445  a(n) = n^4 + n^3 + n^2 + n.
 * https://oeis.org/A027445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2+1) // a(n) = n^4 + n^3 + n^2 + n
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;        /* cast index to sequence type */
    seq_t sn2 = sn * sn;        /* n^2 */
    seq_t part1 = sn + 1;       /* n + 1 */
    seq_t part2 = sn2 + 1;      /* n^2 + 1 */
    return sn * part1 * part2;  /* n * (n+1) * (n^2+1) */
}
