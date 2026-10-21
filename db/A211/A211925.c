/*
 * A211925  Number of lower triangular n X n arrays colored with integers 0
 *          upwards introduced in row major order, with no element equal to
 *          any horizontal, vertical or antidiagonal neighbor, and containing
 *          the value n(n+1)/2-3.
 * https://oeis.org/A211925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3 n^8 + 12 n^7 - 82 n^6 - 96 n^5 + 1035 n^4 - 1468 n^3 -
 *            572 n^2 + 2704 n - 1152) / 384 (for n >= 2; a(1) = 0)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    seq_t N = n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t N4 = N3 * N;
    seq_t N5 = N4 * N;
    seq_t N6 = N5 * N;
    seq_t N7 = N6 * N;
    seq_t N8 = N7 * N;

    seq_t pos = 3 * N8 + 12 * N7 + 1035 * N4 + 2704 * N;
    seq_t neg = 82 * N6 + 96 * N5 + 1468 * N3 + 572 * N2 + 1152;

    seq_t numer = pos - neg;
    seq_t denom = 384;
    seq_t res = numer / denom;

    return res;
}
