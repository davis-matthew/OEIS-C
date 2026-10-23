/*
 * A157918  a(n) = 625*n^2 - 25.
 * https://oeis.org/A157918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 625*n^2 - 25
 * Range:     u8 none, u16 n <= 10, u32 n <= 2621, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset: a(1) is the first defined term */
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn;           /* n^2 */
    val *= (seq_t)625;             /* 625*n^2 */
    val -= (seq_t)25;              /* -25 */
    return val;
}
