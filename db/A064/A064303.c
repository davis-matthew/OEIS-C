/*
 * A064303  Seventh diagonal of triangle A064094.
 * https://oeis.org/A064303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 5*n + 14*n^2 + 28*n^3 + 42*n^4 + 42*n^5
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 39, u64 n <= 3376,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t result = (seq_t)1;

    /* 5*n */
    result += (seq_t)5 * N;

    /* 14*n^2 */
    seq_t N2 = N * N;
    result += (seq_t)14 * N2;

    /* 28*n^3 */
    seq_t N3 = N2 * N;
    result += (seq_t)28 * N3;

    /* 42*n^4 */
    seq_t N4 = N3 * N;
    result += (seq_t)42 * N4;

    /* 42*n^5 */
    seq_t N5 = N4 * N;
    result += (seq_t)42 * N5;

    return result;
}
