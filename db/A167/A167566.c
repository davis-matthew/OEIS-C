/*
 * A167566  The third left hand column of triangle A167565.
 * https://oeis.org/A167566
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 149, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167566: a(n) = (7*n^5 - 30*n^4 + 45*n^3 - 30*n^2 + 8*n)/120
   Offset: 3  (a(3) = 2) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* terms below offset are undefined – return 0 for safety */
    if (n < 3) return 0;

    /* use signed 128‑bit to hold intermediate computations safely */
    signed __int128 nn = (signed __int128)n;

    signed __int128 term1 = 7   * nn * nn * nn * nn * nn;   /* 7*n^5   */
    signed __int128 term2 = 30  * nn * nn * nn * nn;        /* 30*n^4  */
    signed __int128 term3 = 45  * nn * nn * nn;            /* 45*n^3  */
    signed __int128 term4 = 30  * nn * nn;                  /* 30*n^2  */
    signed __int128 term5 = 8   * nn;                       /* 8*n     */

    signed __int128 numerator = term1 - term2 + term3 - term4 + term5;

    /* 5! = 120 */
    signed __int128 value = numerator / 120;

    return (seq_t)value;
}
