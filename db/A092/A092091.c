/*
 * A092091  Molien series for 9-dimensional group of structure Z_2 X Z_2 and
 *          order 4, corresponding to complete weight enumerators of Hermitian
 *          self-dual GF(3)-linear codes over GF(9).
 * https://oeis.org/A092091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((315*(857+167*(-1)^n) + 60*(8347+581*(-1)^n)*n * +
 *            (384718+6930*(-1)^n)*n^2 + 84*(2027+5*(-1)^n)*n^3 * + 48888*n^4
 *            + 9240*n^5 + 1092*n^6 + 72*n^7 + 2*n^8) * / 322560
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 67, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* (-1)^n */
    __int128 sign = (n & 1ULL) ? -1 : 1;

    __int128 nn  = n;
    __int128 n2  = nn * nn;
    __int128 n3  = n2 * nn;
    __int128 n4  = n3 * nn;
    __int128 n5  = n4 * nn;
    __int128 n6  = n5 * nn;
    __int128 n7  = n6 * nn;
    __int128 n8  = n7 * nn;

    __int128 num =
        (__int128)315 * (857 + 167 * sign) +
        (__int128)60  * (8347 + 581 * sign) * nn +
        (__int128)(384718 + 6930 * sign) * n2 +
        (__int128)84  * (2027 + 5   * sign) * n3 +
        (__int128)48888 * n4 +
        (__int128)9240  * n5 +
        (__int128)1092  * n6 +
        (__int128)72    * n7 +
        (__int128)2     * n8;

    __int128 res = num / 322560;   /* exact division */

    return (seq_t)res;
}
