/*
 * A187190  Number of 3-turn rook's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A187190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n^4 - 10*n^3 + 6*n^2
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t N4 = N2 * N2;

    return (seq_t)4 * N4 - (seq_t)10 * N3 + (seq_t)6 * N2;
}
