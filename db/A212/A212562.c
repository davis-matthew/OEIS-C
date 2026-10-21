/*
 * A212562  Number of (w,x,y,z) with all terms in {1,...,n} and w+x<2y+2z.
 * https://oeis.org/A212562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/96*(82*n^4 + 12*n^3 + 8*n^2 + 6*((-1)^n-1)*n - 3*(-1)^n
 *            + 3)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 266, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    __int128 nn = n;
    __int128 n2 = nn * nn;           /* n^2 */
    __int128 n4 = n2 * n2;           /* n^4 */
    __int128 num = 82 * n4 + 12 * nn * n2 + 8 * n2;
    int parity = (n & 1) ? -1 : 1;   /* (-1)^n */
    num += 6 * (__int128)(parity - 1) * nn;
    num -= 3 * (__int128)parity;
    num += 3;
    return (seq_t)(num / 96);
}
