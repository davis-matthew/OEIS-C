/*
 * A211840  Number of nonnegative integer arrays of length n+7 with new values
 *          0 upwards introduced in order, no three adjacent elements equal,
 *          and containing the value n+1.
 * https://oeis.org/A211840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3840)*n^10 + (23/2304)*n^9 + (197/1152)*n^8 +
 *            (9899/5760)*n^7 + (14451/1280)*n^6 + (594979/11520)*n^5 +
 *            (8279/48)*n^4 + (138637/320)*n^3 + (583243/720)*n^2 +
 *            (62003/60)*n + 685
 * Range:     u8 none, u16 n <= 3, u32 n <= 17, u64 n <= 188, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the behavior is undefined. */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 acc = 3;          /* coefficient of n^10 after scaling by 11520 */
    acc = acc * nn + 115;              /* + 115 * n^9 */
    acc = acc * nn + 1970;             /* + 1970 * n^8 */
    acc = acc * nn + 19798;            /* + 19798 * n^7 */
    acc = acc * nn + 130059;           /* + 130059 * n^6 */
    acc = acc * nn + 594979;           /* + 594979 * n^5 */
    acc = acc * nn + 1986960;          /* + 1986960 * n^4 */
    acc = acc * nn + 4990932;          /* + 4990932 * n^3 */
    acc = acc * nn + 9331888;          /* + 9331888 * n^2 */
    acc = acc * nn + 11904576;         /* + 11904576 * n   */
    acc = acc * nn + 7891200;          /* + 7891200           */
    acc = acc / 11520u;                /* divide by LCM of denominators */

    return (seq_t)acc;
}
