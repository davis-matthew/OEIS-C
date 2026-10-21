/*
 * A090393  Eighth diagonal (m=7) of triangle A084938; a(n) = A084938(n+7,n) =
 *          (n^7 + 63*n^6 + 1855*n^5 + 34125*n^4 + 438424*n^3 + 3980172*n^2 +
 *          20946960*n)/5040.
 * https://oeis.org/A090393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^7 + 63*n^6 + 1855*n^5 + 34125*n^4 + 438424*n^3 +
 *            3980172*n^2 + 20946960*n) / 5040
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 71, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit intermediate to avoid overflow for the usual
     * range of n (e.g. n ≤ 10000).  The result fits in seq_t
     * whenever it is representable in that type. */
    __int128 N = n;

    __int128 n1 = N;                     /* n            */
    __int128 n2 = n1 * N;                /* n^2          */
    __int128 n3 = n2 * N;                /* n^3          */
    __int128 n4 = n3 * N;                /* n^4          */
    __int128 n5 = n4 * N;                /* n^5          */
    __int128 n6 = n5 * N;                /* n^6          */
    __int128 n7 = n6 * N;                /* n^7          */

    __int128 num = n7
                 + (__int128)63   * n6
                 + (__int128)1855 * n5
                 + (__int128)34125 * n4
                 + (__int128)438424 * n3
                 + (__int128)3980172 * n2
                 + (__int128)20946960 * n1;

    return (seq_t)(num / (__int128)5040);
}
