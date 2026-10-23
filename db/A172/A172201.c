/*
 * A172201  Number of ways to place 3 nonattacking amazons (superqueens) on an
 *          n X n board.
 * https://oeis.org/A172201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n = 1, 2, 3, 4. For even n >= 4: a(n) = (2*n^6 -
 *            20*n^5 + 31*n^4 + 314*n^3 - 1452*n^2 + 2040*n - 672) / 12 For
 *            odd n >= 5: a(n) = (2*n^6 - 20*n^5 + 31*n^4 + 314*n^3 - 1452*n^2
 *            + 2034*n - 669) / 12 This matches OEIS A172201
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 55, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;

    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;
    seq_t n3  = n2 * N;
    seq_t n4  = n3 * N;
    seq_t n5  = n4 * N;
    seq_t n6  = n5 * N;

    seq_t num;
    if ((n & 1) == 0)  /* even n */
        num = 2 * n6 - 20 * n5 + 31 * n4 + 314 * n3 - 1452 * n2 + 2040 * N - 672;
    else                /* odd n */
        num = 2 * n6 - 20 * n5 + 31 * n4 + 314 * n3 - 1452 * n2 + 2034 * N - 669;

    return num / 12;
}
