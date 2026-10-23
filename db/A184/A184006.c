/*
 * A184006  floor(nr+h), where r=sqrt(3), h=-1/3; complement of A184007.
 * https://oeis.org/A184006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(3) - 1/3)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute floor(n*sqrt(3)) as floor(sqrt(3*n^2)) */
    idx_t nn = n;
    __int128 m = (__int128)nn * nn * 3;          /* 3*n^2  (128‑bit to avoid overflow) */

    /* binary search for sqrt(m).  Since sqrt(3) < 2, the result < 2*n+1. */
    idx_t low = 0, high = nn * 2 + 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= m)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t k = low;   /* k = floor(n*sqrt(3)) */

    /* decide whether to subtract 1.
       Condition f < 1/3 is equivalent to:
          27*n^2 - 9*k^2 - 6*k - 1 < 0
       where f = n*sqrt(3) - k. */
    __int128 U = (__int128)27 * nn * nn
                 - (__int128)9 * k * k
                 - (__int128)6 * k
                 - 1;

    if (U < 0)
        return (seq_t)(k - 1);   /* floor(n*sqrt(3) - 1/3) = k-1 */
    else
        return (seq_t)k;          /* otherwise it equals k */
}
