/*
 * A157534  Half the number of length n integer sequences with sum zero and
 *          sum of squares 8.
 * https://oeis.org/A157534
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            n*(n-1)*(n^6-27*n^5+295*n^4-1473*n^3+3376*n^2-3036*n+1008)/1152
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 19, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset (n < 2) – behaviour is irrelevant there. */
    if (n < 2) return (seq_t)0;

    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t N4 = N3 * N;
    seq_t N5 = N4 * N;
    seq_t N6 = N5 * N;

    seq_t poly = N6;
    poly = poly - (seq_t)27 * N5;
    poly = poly + (seq_t)295 * N4;
    poly = poly - (seq_t)1473 * N3;
    poly = poly + (seq_t)3376 * N2;
    poly = poly - (seq_t)3036 * N;
    poly = poly + (seq_t)1008;

    seq_t term = N * (N - 1);
    seq_t num  = term * poly;

    return num / (seq_t)1152;
}
