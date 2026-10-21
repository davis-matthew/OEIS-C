/*
 * A076461  Sum of squares of numbers that can be written as t*n + u*(n+1) for
 *          nonnegative integers t,u in exactly two ways.
 * https://oeis.org/A076461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)*(25*n^4+26*n^3-6*n^2-7*n+1)/6
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 31, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safe guard – offset is 1 */
    unsigned __int128 sn   = (unsigned __int128) n;
    unsigned __int128 n1   = sn + 1;
    unsigned __int128 term = sn * n1;                           /* n*(n+1) */

    /* Polynomial: 25*n^4 + 26*n^3 - 6*n^2 - 7*n + 1 */
    unsigned __int128 n2   = sn * sn;                           /* n^2 */
    unsigned __int128 n3   = n2 * sn;                           /* n^3 */
    unsigned __int128 n4   = n3 * sn;                           /* n^4 */

    unsigned __int128 poly = 25 * n4 + 26 * n3 - 6 * n2 - 7 * sn + 1;

    unsigned __int128 prod = term * poly;
    prod /= 6;                                                   /* exact division */

    return (seq_t) prod;                                           /* cast back */
}
