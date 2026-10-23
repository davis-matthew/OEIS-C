/*
 * A156580  Triangle T(n, k) = binomial(n-1, k-1) for n < 4, 2*(n + k - 4) +
 *          (-1)^n if k = (n+1)/2, 2*(n + k - 4) if k < floor(n/2) + 1,
 *          otherwise 2*(2*n - k - 3), with T(n, 0) = T(n, n) = 1, read by
 *          rows.
 * https://oeis.org/A156580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m, k) m = smallest integer with m*(m+1)/2 >= n k = n -
 *            (m-1)*m/2 T(n, k) is defined by the piecewise rules in the OEIS
 *            description (binomial for n < 4, and the formulas for n ≥ 4).
 *            The result fits into seq_t when it fits into its natural range
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* find the row m such that m*(m+1)/2 >= n  */
    idx_t lo = 1, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 < n) hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n) hi = mid;
        else lo = mid + 1;
    }
    idx_t m = lo;

    /* column index k in the row m */
    idx_t tri_prev = (unsigned __int128)(m - 1) * m / 2;
    idx_t k = n - tri_prev;

    /* compute T(m, k) */
    seq_t result;
    if (m < 4) {                      /* binomial for n < 4 */
        if (k == 1 || k == m)         result = (seq_t)1;
        else if (m == 3 && k == 2)    result = (seq_t)2;
        else                          result = (seq_t)1;   /* safety */
    } else {
        if (k == 1 || k == m)         result = (seq_t)1;
        else {
            idx_t limit = m / 2 + 1;   /* floor(n/2)+1   */
            if ((m & 1) && k == (m + 1) / 2) {   /* middle for odd n */
                unsigned __int128 temp = 2 * ((unsigned __int128)m + k - 4);
                result = (seq_t)(temp - 1);      /* 2*(n+k-4) - 1 */
            } else if (k < limit) {
                unsigned __int128 temp = 2 * ((unsigned __int128)m + k - 4);
                result = (seq_t)temp;
            } else {
                unsigned __int128 temp = 2 * ((unsigned __int128)(2 * m - k - 3));
                result = (seq_t)temp;
            }
        }
    }
    return result;
}
