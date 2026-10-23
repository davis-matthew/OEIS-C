/*
 * A156578  Triangle of coefficients of 1 - (n+1)*x^n + n*x^(n+1), read by
 *          rows.
 * https://oeis.org/A156578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, less
 * Formula:   a(n) = value of the irregular triangle T(r, k) read by rows, *
 *            where for r > 0 * T(r,0) = 1 * T(r,r) = -(r+1) * T(r,r+1) = r *
 *            all other entries are 0. The triangle has rows * r = 0 : [0] * r
 *            = 1 : [1, -2, 1] * r = 2 : [1, 0, -3, 2] * … * a(n) is the n-th
 *            element of the concatenation of these row...
 * Range:     i8 n <= 1375, i16 n <= 1375, i32 n <= 1375, i64 n <= 1375,
 *            i128 n <= 1375, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: floor of sqrt for a 128‑bit integer. */
static inline idx_t isqrt128(__int128 x)
{
    idx_t low  = 0;
    idx_t high = 1;
    while ((__int128)high * high <= x) {
        high <<= 1;
    }
    low = high >> 1;
    while (high > low + 1) {
        idx_t mid = (low + high) >> 1;
        if ((__int128)mid * mid <= x) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Special case n == 0 – row 0 returns 0. */
    if (n == 0) return (seq_t)0;

    /* Compute the row number r such that
     * (r-1)^2 + 5(r-1) + 2 / 2 <= n < (r^2 + 5r + 2) / 2   (r ≥ 1) */
    __int128 temp = (__int128)8 * n + 17;             /* 8n + 17 >= 17 */
    idx_t sqrtD   = isqrt128(temp);                   /* floor(sqrt(8n+17)) */
    idx_t r        = ((sqrtD + 1) >> 1) - 2;           /* row number (r ≥ 0) */

    /* Start index of row r */
    idx_t prev;                                       /* elements before row r */
    if (r == 0)
        prev = 0;
    else {
        idx_t r1 = r - 1;
        prev = ( ( (__int128)r1 * r1 + 5 * r1 + 2 ) >> 1 );
    }

    /* Position within the row */
    idx_t k = n - prev;                               /* 0 ≤ k < row length */

    if (r == 0) return (seq_t)0;                      /* only element 0 */

    if (k == 0)          return (seq_t)1;              /* first element of the row */
    if (k == r)          return (seq_t)-( (seq_t)(r + 1) ); /* middle element */
    if (k == r + 1)      return (seq_t)(r);           /* last element */

    return (seq_t)0;                                 /* all other entries are 0 */
}
