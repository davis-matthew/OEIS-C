/*
 * A156690  Triangle T(n, k, m) = t(n,m)/( t(k,m) * t(n-k,m) ) with T(n, 0, m)
 *          = T(n, n, m) = 1, where t(n, m) = Product_{j=1..n}
 *          Product_{i=1..j-1} ( 1 - (m+1)*(i+1) ) and m = 1, read by rows.
 * https://oeis.org/A156690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) with m = 1. Find r such that * r(r+1)/2 <= n <
 *            (r+1)(r+2)/2 (row index) * Let k = n - r(r+1)/2 (column index).
 *            * Then a(n) = t(r)/(t(k)*t(r-k)) where * t(n) =
 *            Product_{i=1}^{n-1} (-(2i+1))^{n-i}. * The implementation
 *            multiplies each odd factor the required * number of times,
 *            keeping...
 * Range:     i8 n <= 11, i16 n <= 16, i32 n <= 30, i64 n <= 48, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find the row index r: r(r+1)/2 <= n < (r+1)(r+2)/2   */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* column index k */
    idx_t k = n - r * (r + 1) / 2;

    seq_t result = 1;            /* multiplicative result */
    unsigned sign_parity = 0;    /* parity of the number of negative factors */

    /* iterate over i = 1 .. r-1 */
    for (idx_t i = 1; i < r; ++i)
    {
        /* number of times the factor appears in t(r),
         * t(k) (if i < k) and t(r-k) (if i < r-k) are subtracted. */
        idx_t term_k   = (k > i)   ? (k - i)   : 0;     /* k - i if i < k */
        idx_t term_rk  = ((r - k) > i) ? ((r - k) - i) : 0;

        idx_t e = (r - i) - term_k - term_rk;   /* exponent of (2i+1)   */
        if (e == 0)
            continue;

        /* update sign: each negative factor contributes one to the
         * parity of the exponent. */
        sign_parity ^= (unsigned)(e & 1);

        /* multiply (2i+1) e times */
        seq_t base = (seq_t)(2 * i + 1);
        for (idx_t j = 0; j < e; ++j)
            result *= base;
    }

    if (sign_parity)
        result = -result;

    return result;
}
