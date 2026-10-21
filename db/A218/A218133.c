/*
 * A218133  Number of length 10 primitive (=aperiodic or period 10) n-ary
 *          words.
 * https://oeis.org/A218133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^10 - n^5 - n^2 + n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 84, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s = (seq_t)n;
    seq_t s2 = s * s;          /* n^2 */
    seq_t s4 = s2 * s2;        /* n^4 */
    seq_t s5 = s4 * s;         /* n^5 */
    seq_t s10 = s5 * s5;       /* n^10 */
    seq_t part1 = s10 + s;      /* n^10 + n */
    seq_t part2 = s5 + s2;      /* n^5 + n^2 */
    return part1 - part2;
}
