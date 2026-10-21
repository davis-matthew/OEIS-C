/*
 * A223523  Triangle S(n, k) by rows: coefficients of
 *          2^((n-1)/2)*(x^(1/2)*d/dx)^n, where n = 1, 3, 5, ...
 * https://oeis.org/A223523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flattening of triangle S(n,k) for odd n, where S(2m+1,k)
 *            = T(m, m-k) and T(m,t) = 2^m * m!/(m-t)! * C(m+1/2, t) (1 ≤ t ≤
 *            m, 0 ≤ t ≤ m)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

extern seq_t factorial(idx_t n);

/* Find smallest r such that r*(r+1)/2 > n */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = n + 1;          /* high is exclusive bound */
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 lhs = (unsigned __int128)mid * (mid + 1) / 2;
        if (lhs > n)
            high = mid;
        else
            low = mid;
    }
    return high;                          /* r = high */
}

seq_t a(idx_t n)
{
    /* Step 1: locate the row (r) and position within the row (p) */
    idx_t r = find_row(n);                        /* r >= 1, row length = r */
    idx_t offset = n - ((r - 1) * r) / 2;         /* 0 <= offset < r */
    idx_t m = r - 1;                              /* m = (n_real-1)/2 */
    idx_t p = offset;                             /* column index */
    idx_t t = m - p;                              /* t = m - p */

    /* Step 2: compute T(m,t) = 2^{m-t} * (∏_{j=0}^{t-1} (m-j)(2m+1-2j)) / t! */
    seq_t res = 1;

    for (idx_t j = 0; j < t; ++j) {
        /* multiply by (m-j) */
        res *= (seq_t)(m - j);

        /* multiply by (2m+1-2j) */
        res *= (seq_t)(2 * (seq_t)m + 1 - 2 * (seq_t)j);

        /* divide by (j+1) whenever possible to keep the intermediate size small */
        seq_t div = (seq_t)(j + 1);
        if (res % div == 0)
            res /= div;
    }

    /* multiply by 2^{m-t} */
    res *= pow2(m - t);

    return res;
}
