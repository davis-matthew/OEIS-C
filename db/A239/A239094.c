/*
 * A239094  a(n) = (5*n^9 - 30*n^7 + 63*n^5 - 50*n^3 + 12*n)/360.
 * https://oeis.org/A239094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n^9 - 30*n^7 + 63*n^5 - 50*n^3 + 12*n)/360
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 18, u64 n <= 222, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use a 128‑bit intermediate to avoid overflow for n up to ~10⁴ */
    __int128 x  = n;
    __int128 n2 = x * x;      /* n²   */
    __int128 n3 = n2 * x;     /* n³   */
    __int128 n4 = n2 * n2;    /* n⁴   */
    __int128 n5 = n4 * x;     /* n⁵   */
    __int128 n6 = n4 * n2;    /* n⁶   */
    __int128 n7 = n6 * x;     /* n⁷   */
    __int128 n8 = n4 * n4;    /* n⁸   */
    __int128 n9 = n8 * x;     /* n⁹   */

    __int128 num = 5 * n9 - 30 * n7 + 63 * n5 - 50 * n3 + 12 * x;
    __int128 res = num / 360; /* the division is exact for all integer n */

    return (seq_t)res;
}
