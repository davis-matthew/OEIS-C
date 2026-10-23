/*
 * A181385  Maximal number that can be obtained by reversing n in an integer
 *          base.
 * https://oeis.org/A181385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = ceil(n^2/2) - floor(n^2/4) for n != 2, a(2) = 2
 * Range:     u8 n <= 15, u16 n <= 416, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2)
        return (seq_t)2;

    seq_t sq = (seq_t)n * (seq_t)n;
    /* ceil(n^2 / 2) == (n^2 + 1) / 2 */
    seq_t ceil_half   = (sq + (seq_t)1) / (seq_t)2;
    /* floor(n^2 / 4) == n^2 / 4 */
    seq_t floor_quarter = sq / (seq_t)4;
    return ceil_half - floor_quarter;
}
