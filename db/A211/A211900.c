/*
 * A211900  Number of triangular nXnXn arrays colored with integers 0 upwards
 *          introduced in row major order, with no element equal to any
 *          neighbor, and containing the value n(n+1)/2-3.
 * https://oeis.org/A211900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n = 1 a(n) = (1/128)n^8 + (1/32)n^7 - (41/192)n^6 -
 *            (3/8)n^5 + (329/128)n^4 - (139/96)n^3 - (467/96)n^2 + (127/24)n
 *            (n>1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    unsigned __int128 nn   = n;
    unsigned __int128 n2   = nn * nn;
    unsigned __int128 n3   = n2 * nn;
    unsigned __int128 n4   = n3 * nn;
    unsigned __int128 n5   = n4 * nn;
    unsigned __int128 n6   = n5 * nn;
    unsigned __int128 n7   = n6 * nn;
    unsigned __int128 n8   = n7 * nn;

    unsigned __int128 num =
            (unsigned __int128)3   * n8   /* 1/128 */
          + (unsigned __int128)12  * n7   /* 1/32  */
          - (unsigned __int128)82  * n6   /* -41/192 */
          - (unsigned __int128)144 * n5   /* -3/8   */
          + (unsigned __int128)987 * n4   /* 329/128*/
          - (unsigned __int128)556 * n3   /* -139/96*/
          - (unsigned __int128)1868* n2   /* -467/96*/
          + (unsigned __int128)2032 * nn; /* 127/24 */

    num /= 384;   /* common denominator LCM(128,32,192,8,128,96,96,24) */

    return (seq_t)num;
}
