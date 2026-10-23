/*
 * A159356  Number of n X n arrays of squares of integers with every
 *          (n-1)X(n-1) subblock summing to 4.
 * https://oeis.org/A159356
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/288*(519*n^8 - 6716*n^7 + 34142*n^6 - 78248*n^5 +
 *            32735*n^4 * + 236596*n^3 - 555412*n^2 + 518112*n - 185184), *
 *            for n >= 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 16, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;               /* behaviour for the offset is irrelevant */

    __int128 nn = n;             /* unsigned idx_t fits in signed __int128 */
    __int128 res = 519;          /* coefficient of n^8 */

    res = res * nn - 6716;
    res = res * nn + 34142;
    res = res * nn - 78248;
    res = res * nn + 32735;
    res = res * nn + 236596;
    res = res * nn - 555412;
    res = res * nn + 518112;
    res = res * nn - 185184;
    res /= 288;                  /* exact division */

    return (seq_t) res;          /* cast to the required unsigned type */
}
