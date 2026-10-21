/*
 * A212505  Number of (w,x,y,z) with all terms in {1,...,n} and w<2x and
 *          y>=2z.
 * https://oeis.org/A212505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^2/4) * ceil(3n^2/4) ceil(x) for positive integers
 *            x can be computed as (x + k -1)/k. Here k = 4, so ceil(3n^2/4) =
 *            (3n^2 + 3)/4
 * Range:     u8 n <= 6, u16 n <= 24, u32 n <= 389, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nsq = (seq_t)n * (seq_t)n;            /* n^2 */
    seq_t part1 = nsq / 4;                      /* floor(n^2/4) */
    seq_t part2 = ((seq_t)3 * nsq + 3) / 4;      /* ceil(3n^2/4)  */
    return part1 * part2;
}
