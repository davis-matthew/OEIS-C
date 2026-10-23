/*
 * A199899  Number of -n..n arrays x(0..3) of 4 elements with zero sum, and
 *          adjacent elements not both strictly positive and not both strictly
 *          negative.
 * https://oeis.org/A199899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n^3 + 18*n^2 + 20*n + 3) / 3 (OEIS A199899)
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* OEIS offset is 1; for n == 0 return 0 (unused case). */
    if (n == 0)
        return (seq_t)0;

    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;

    seq_t num = (seq_t)4 * n3 + (seq_t)18 * n2 + (seq_t)20 * N + (seq_t)3;
    return num / (seq_t)3;
}
