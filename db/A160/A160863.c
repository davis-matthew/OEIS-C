/*
 * A160863  Expansion of
 *          (1+147*x+1142*x^2+1717*x^3+656*x^4+60*x^5+x^6)/(1-x)^7.
 * https://oeis.org/A160863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 931*n^6/180 +1261*n^5/60 +1547*n^4/36 +565*n^3/12
 *            +1276*n^2/45 +42*n/5 +1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 29, u64 n <= 1235,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* cast index to a generic integer type */
    seq_t sn = (seq_t)n;                           /* to match seq_t width   */
    unsigned __int128 N = (unsigned __int128)sn;   /* 128‑bit workspace     */

    /* compute powers of n in 128‑bit arithmetic */
    unsigned __int128 n2 = N * N;          /* n^2 */
    unsigned __int128 n3 = n2 * N;         /* n^3 */
    unsigned __int128 n4 = n3 * N;         /* n^4 */
    unsigned __int128 n5 = n4 * N;         /* n^5 */
    unsigned __int128 n6 = n5 * N;         /* n^6 */

    /* accumulate the common numerator (denominator 180) */
    unsigned __int128 num =
          (unsigned __int128)931 * n6
        + (unsigned __int128)3783 * n5   /* 1261*3   */
        + (unsigned __int128)7735 * n4   /* 1547*5   */
        + (unsigned __int128)8475 * n3   /* 565*15   */
        + (unsigned __int128)5104 * n2   /* 1276*4   */
        + (unsigned __int128)1512 * N    /* 42*36    */
        + (unsigned __int128)180;        /* constant term   */

    return (seq_t)(num / 180);
}
