/*
 * A035878  Number of points of l_1 norm n in the "diamond" lattice D^+_4.
 * https://oeis.org/A035878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2 + 1 + (n^2 + 2)*(-1)^n)*4*n/3 (for n>0), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if (N == 0)
        return 1;

    if ((N & (seq_t)1) == 0) {           /* n even */
        seq_t temp = N * N + 1;           /* N^2 + 1 */
        return 4 * N * temp;              /* 4 * N * (N^2 + 1) */
    } else {                              /* n odd */
        seq_t temp = N * N - 1;           /* N^2 - 1 */
        temp = temp * 4;                  /* (N^2 - 1) * 4 */
        temp = temp * N;                  /* ((N^2 - 1) * 4) * N */
        return temp / 3;                  /* /3 exactly */
    }
}
