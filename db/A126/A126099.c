/*
 * A126099  Number of 3-indecomposable (connected) graphs on n nodes.
 * https://oeis.org/A126099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*n + 2*n)/8 for even n>7, a(n) = (n*n - 1)/8 for odd
 *            n>7
 * Range:     u8 n <= 45, u16 n <= 723, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Special values for n <= 7: 1,1,1,2,6,21,7 */

seq_t a(idx_t n)
{
    /* values for n = 1..7, index 0 unused */
    static const seq_t small[8] = {0,1,1,1,2,6,21,7};

    if (n <= 7)
        return small[(int)n];

    seq_t N = (seq_t)n;
    if ((n & 1) == 0) {           /* even */
        return (N*N + 2*N) / 8;
    } else {                       /* odd */
        return (N*N - 1) / 8;
    }
}
