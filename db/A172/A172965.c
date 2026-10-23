/*
 * A172965  Number of ways to place 3 nonattacking knights on an n X n
 *          cylindrical board.
 * https://oeis.org/A172965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n - 3)*(n^4 + 3*n^3 - 18*n^2 - 18*n + 164)/6, n >= 6.
 *            Early terms: a(1)=0, a(2)=0, a(3)=6, a(4)=240, a(5)=1010
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 53, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle early terms directly */
    if (n <= 2) return (seq_t)0;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)240;
    if (n == 5) return (seq_t)1010;

    /* n >= 6: compute using the closed-form formula */
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;          /* n^2 */
    seq_t N3 = N2 * N;         /* n^3 */
    seq_t N4 = N2 * N2;        /* n^4 */

    seq_t poly = N4
                 + (seq_t)3 * N3
                 - (seq_t)18 * N2
                 - (seq_t)18 * N
                 + (seq_t)164;

    seq_t result = N * (N - 3) * poly;
    result /= (seq_t)6;

    return result;
}
