/*
 * A055358  Number of increasing mobiles (circular rooted trees) with n nodes
 *          and 4 leaves.
 * https://oeis.org/A055358
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * 31*2^(2*n - 3)/9 - 5*(n+2)*3^n/12 + (n^2 + 2*n +
 *            2)*2^(n-2) * - n^3/6 - n/4 - 1/18 * (as given in the OEIS entry)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n < 5)
        return (seq_t)0;

    seq_t sn = (seq_t)n;

    /*  Exponents for the powers of 2 and 3   */
    idx_t e1 = 2 * n - 3;   /* exponent for 2^(2n-3) */
    idx_t e2 = n;         /* exponent for 3^n          */
    idx_t e3 = n - 2;     /* exponent for 2^(n-2)      */

    /*  Compute the powers – these are seq_t values  */
    seq_t p1 = pow2(e1);   /* 2^(2n-3) */
    seq_t p2 = pow3(e2);   /* 3^n     */
    seq_t p3 = pow2(e3);   /* 2^(n-2) */

#ifdef __SIZEOF_INT128__
    /*  For 64‑bit seq_t (or smaller) use 128‑bit intermediate
     *  to avoid overflow before the final division by 36.
     */
    if (sizeof(seq_t) <= 8) {
        __int128 N = 0;

        N += (__int128)124 * (__int128)p1;                                   /* 124 = 31*4  */
        N -= (__int128)15  * (__int128)(sn + 2) * (__int128)p2;               /* 15  = 5*3   */
        N += (__int128)36  * (__int128)(sn * sn + 2 * sn + 2) * (__int128)p3; /* 36  = 4*9   */
        N -= (__int128)6   * (__int128)sn * (__int128)sn * (__int128)sn;     /* 6   = 36/6  */
        N -= (__int128)9   * (__int128)sn;                                  /* 9   = 36/4  */
        N -= (__int128)2;                                                  /* 2   = 36/18 */

        return (seq_t)(N / 36);
    }
#endif

    /*  For arbitrary‑precision seq_t (or 128‑bit seq_t) we
     *  perform all arithmetic directly with seq_t.
     */
    seq_t N = 0;

    N += (seq_t)124 * p1;
    N -= (seq_t)15 * (sn + 2) * p2;
    N += (seq_t)36 * (sn * sn + 2 * sn + 2) * p3;
    N -= (seq_t)6 * sn * sn * sn;
    N -= (seq_t)9 * sn;
    N -= (seq_t)2;

    return N / 36;
}
