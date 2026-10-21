/*
 * A212022  Number of lower triangular n X n arrays colored with integers 0
 *          upwards introduced in row major order, with no element equal to
 *          any horizontal, diagonal or antidiagonal neighbor, and containing
 *          the value n(n+1)/2-3.
 * https://oeis.org/A212022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/128)*n^8 + (1/32)*n^7 - (41/192)*n^6 - (1/4)*n^5 * +
 *            (345/128)*n^4 - (367/96)*n^3 - (47/96)*n^2 + (97/24)*n - 1 *
 *            (empirical polynomial for n>1, a(1)=0)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;

    /* use signed 128‑bit integer for safety */
    __int128 N  = n;          /* n */
    __int128 n2 = N * N;      /* n^2 */
    __int128 n3 = n2 * N;     /* n^3 */
    __int128 n4 = n2 * n2;    /* n^4 */
    __int128 n5 = n4 * N;     /* n^5 */
    __int128 n6 = n4 * n2;    /* n^6 */
    __int128 n7 = n6 * N;     /* n^7 */
    __int128 n8 = n4 * n4;    /* n^8 */

    /* numerator of the polynomial expressed with common denominator 384 */
    __int128 numer = 3 * n8
                   + 12 * n7
                   - 82 * n6
                   - 96 * n5
                   + 1035 * n4
                   - 1468 * n3
                   - 188 * n2
                   + 1552 * N
                   - 384;

    /* the numerator is divisible by 384 for all n >= 2 */
    __int128 res = numer / 384;

    return (seq_t)res;
}
