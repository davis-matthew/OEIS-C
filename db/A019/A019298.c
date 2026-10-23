/*
 * A019298  Number of balls in pyramid with base either a regular hexagon or a
 *          hexagon with alternate sides differing by 1 (balls in hexagonal
 *          pyramid of height n taken from hexagonal close-packing).
 * https://oeis.org/A019298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor((n^2+1)(2n+3)/8)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;               /* convert index to seq_t */
    seq_t part1 = nn * nn + 1;         /* (n^2 + 1) */
    seq_t part2 = 2 * nn + 3;          /* (2n + 3) */
    seq_t prod  = part1 * part2;       /* (n^2+1)(2n+3) */
    return prod / 8;                   /* floor division by 8 */
}
