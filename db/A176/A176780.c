/*
 * A176780  a(n) = n^4 + 6n^3 + 14n^2 + 15n + 6.
 * https://oeis.org/A176780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 + 6*n^3 + 14*n^2 + 15*n + 6
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 254, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t before any arithmetic */
    seq_t res = 1;                /* leading coefficient of n^4 */
    res = res * nn + 6;           /* 1*n + 6   -> n + 6                */
    res = res * nn + 14;          /* (n + 6)*n + 14 -> n^2 + 6n + 14      */
    res = res * nn + 15;          /* (n^2 + 6n + 14)*n + 15 -> n^3 + 6n^2 + 14n + 15 */
    res = res * nn + 6;           /* last multiplication => desired polynomial */
    return res;
}
