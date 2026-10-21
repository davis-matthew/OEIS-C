/*
 * A072474  Sum of next n squares.
 * https://oeis.org/A072474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n/12)*(3*n^2 + 1)*(n^2 + 2) (OEIS A072474)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 111, u64 n <= 9410,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* casting n to a larger type to avoid overflow in intermediate products */
    unsigned __int128 N = n;

    /* compute numerator: n * (3*n*n + 1) * (n*n + 2) */
    unsigned __int128 tmp = N * (3 * N * N + 1);
    tmp *= (N * N + 2);

    /* divide by 12 - the result is always an integer */
    tmp /= 12;

    return (seq_t)tmp;
}
