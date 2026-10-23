/*
 * A103532  Number of divisors of 240^n.
 * https://oeis.org/A103532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (4*n+1)*(n+1)^2
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t part1 = 4*nn + 1;          /* 4*n + 1 */
    seq_t part2 = (nn + 1)*(nn + 1); /* (n+1)^2 */
    return part1 * part2;
}
