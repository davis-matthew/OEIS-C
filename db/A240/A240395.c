/*
 * A240395  Number of 3 X n 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or one plus the sum of the elements
 *          above it or two plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A240395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 - 5*n^3 + 26*n^2 + 62*n - 288)/6 for n > 5, a(1)=2,
 *            a(2)=12, a(3)=32, a(4)=62, a(5)=118
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small initial values (no array needed) */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)32;
    if (n == 4) return (seq_t)62;
    if (n == 5) return (seq_t)118;

    /* compute using idx_t to avoid overflow */
    idx_t N  = n;
    idx_t N2 = N * N;
    idx_t N3 = N2 * N;
    idx_t N4 = N2 * N2;
    idx_t numer = N4 - 5 * N3 + 26 * N2 + 62 * N - 288;
    idx_t res = numer / 6;
    return (seq_t)res;
}
