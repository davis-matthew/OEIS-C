/*
 * A228637  The number triangle associated with the polynomials V_n(x).
 * https://oeis.org/A228637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = V_i(j) where i + j = k and k(k+1)/2 <= n < (k+1)(k+2)/2.
 *            Let d = n - k(k+1)/2, then j = d, i = k - d. V_0(x) = 1 V_1(x) =
 *            2x - 1 V_{m+1}(x) = 2xV_m(x) - V_{m-1}(x) (m >= 1)
 * Range:     i8 n <= 22, i16 n <= 47, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find k such that k*(k+1)/2 <= n < (k+1)*(k+2)/2 */
    idx_t low = 0, high = n, k = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 s = (__int128)mid * (mid + 1) / 2;
        if (s <= n) {
            k = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;          /* avoid underflow */
            high = mid - 1;
        }
    }

    __int128 sk128 = (__int128)k * (k + 1) / 2;
    idx_t sk = (idx_t)sk128;   /* safe: sk <= n <= UINT64_MAX */
    idx_t d = n - sk;          /* offset inside the antidiagonal */
    idx_t i = k - d;           /* degree of the polynomial */
    idx_t j = d;               /* value at which to evaluate */

    /* Evaluate V_i(j) using the recurrence */
    if (i == 0) return (seq_t)1;
    if (i == 1) return (seq_t)2 * (seq_t)j - (seq_t)1;

    seq_t v0 = 1;                             /* V_0(j) */
    seq_t v1 = (seq_t)2 * (seq_t)j - (seq_t)1;/* V_1(j) */
    for (idx_t m = 2; m <= i; ++m) {
        seq_t v2 = ((seq_t)2 * (seq_t)j) * v1 - v0; /* V_m(j) */
        v0 = v1;
        v1 = v2;
    }
    return v1;
}
