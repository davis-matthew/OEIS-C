/*
 * A176331  Triangle read by rows: T(n, k) = Sum_{j=0..n} C(j, n-k) * C(j, k)
 *          * (-1)^(n - j).
 * https://oeis.org/A176331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 69, u32 n <= 197, u64 n <= 680,
 *            u128 n <= 2376, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A176331
    Triangle read by rows:
    T(n,k) = Σ_{j=0..n} C(j, n-k) * C(j, k) * (-1)^(n-j)
    a(n) = the (n)-th element of that triangle (row‐major order).
*/
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Locate the row `r` such that r*(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = (low + high) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri <= n) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    /* Column index within that row */
    idx_t c = n - r * (r + 1) / 2;
    seq_t sum = 0;

    for (idx_t j = 0; j <= r; ++j) {
        seq_t term = binom(j, r - c);
        term *= binom(j, c);
        if ((r - j) & 1)
            sum -= term;
        else
            sum += term;
    }
    return sum;
}
