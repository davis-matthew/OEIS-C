/*
 * A122432  Riordan array (1/(1+x)^3,x).
 * https://oeis.org/A122432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is the row‑major flattening of the triangle
 *            T(r,c)=(-1)^(r-c)*C(r-c+2,2). Let r be the unique row such that
 *            r(r+1)/2 ≤ n < (r+1)(r+2)/2, and let c = n – r(r+1)/2. Then a(n)
 *            = (-1)^d * C(d+2,2) with d = r – c: a(n) = ((d+2)*(d+1)/2) *
 *            ((d%2==0) ? 1 : -1)
 * Range:     i8 n <= 119, i16 n <= 1274, i32 n <= 1274, i64 n <= 1274,
 *            i128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find r with r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t low = 0;
    idx_t high = 1;
    while (((__int128)high * (high + 1) / 2) <= n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (((__int128)mid * (mid + 1) / 2) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t before = r * (r + 1) / 2;
    idx_t c = n - before;          /* 0 ≤ c ≤ r */
    idx_t d = r - c;               /* 0 ≤ d ≤ r */

    seq_t val = ((seq_t)(d + 2) * (d + 1)) / 2;/* C(d+2,2) */
    if (d & 1)                          /* apply sign (-1)^d */
        val = -val;
    return val;
}
