/*
 * A035005  Number of possible queen moves on an n X n chessboard.
 * https://oeis.org/A035005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n-1)*2*n^2 + (4*n^3-6*n^2+2*n)/3, n ≥ 1
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* value below the offset is undefined */
    seq_t nv = (seq_t)n;                  /* convert index to seq_t */
    seq_t part1 = (nv - 1) * 2 * nv * nv; /* (n-1)*2*n^2 */
    seq_t num   = 4 * nv * nv * nv - 6 * nv * nv + 2 * nv;  /* numerator of the second part */
    seq_t part2 = num / 3;                /* divide by 3 – exact division */
    return part1 + part2;
}
