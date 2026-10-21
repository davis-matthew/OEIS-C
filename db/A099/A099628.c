/*
 * A099628  Numbers m where m-th Catalan number A000108(m) =
 *          binomial(2m,m)/(m+1) is divisible by 2 but not by 4, i.e., where
 *          A048881(m) = 1.
 * https://oeis.org/A099628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 2^(r+1) + 2^k - 1, where r is the row number and k the
 *            column index inside the row of the triangle T(r,k) =
 *            2^(r+1)+2^k-1 that enumerates the sequence
 * Range:     u8 n <= 28, u16 n <= 120, u32 n <= 496, u64 n <= 2016,
 *            u128 n <= 8128, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static idx_t floor_sqrt_idx(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = ((idx_t)1 << 32);          /* hi^2 > max uint64_t  */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= x / mid)                 /* mid*mid <= x  */
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* find row r such that (r+1)(r+2)/2 >= n */
    idx_t tmp = 8 * n + 1;                 /* 8*n+1 fits in idx_t for normal n */
    idx_t sqrtInt = floor_sqrt_idx(tmp);
    idx_t r = (sqrtInt > 3u) ? ((sqrtInt - 3u) >> 1) : 0u;

    /* adjust r upward if necessary */
    while (((r + 1u) * (r + 2u)) / 2u < n)
        ++r;

    /* previous cumulative count */
    idx_t N_prev = (r * (r + 1u)) / 2u;
    idx_t k = n - N_prev - 1u;                 /* column inside the row */

    seq_t res = pow2(r + 1u);
    res += pow2(k);
    res -= 1;
    return res;
}
