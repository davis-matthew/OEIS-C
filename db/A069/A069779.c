/*
 * A069779  q-factorial numbers 4!_q.
 * https://oeis.org/A069779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n + 1)*(n^2 + n + 1)*(n^3 + n^2 + n + 1)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    seq_t n2   = sn * sn;            /* n^2 */
    seq_t n3   = n2 * sn;            /* n^3 */
    seq_t part2 = n2 + sn + 1;       /* n^2 + n + 1 */
    seq_t part3 = n3 + n2 + sn + 1;  /* n^3 + n^2 + n + 1 */
    seq_t part1 = sn + 1;            /* n + 1 */
    return part1 * part2 * part3;
}
