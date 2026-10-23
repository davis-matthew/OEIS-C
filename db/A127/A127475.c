/*
 * A127475  Triangle T(n,k) read by rows: T(n,k) = mu(n)*phi(k) if k|n, else
 *          T(n,k)=0.
 * https://oeis.org/A127475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, easy, sign
 * Formula:   T(n,k) = mu(n) * phi(k) if k | n, else 0 a(n) = T(r,k) where n
 *            is the nth element of the triangle read by rows (offset 1)
 * Range:     i8 n <= 91, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   mobius, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required external helpers */
#include "../utils/mobius.h"   /* seq_t mobius(idx_t n); */ 
#include "../utils/phi.h"     /* seq_t phi(idx_t n); */

/* integer square–root for 128‑bit values, used in row calculation */
static inline idx_t isqrt_u128(__uint128_t x)
{
    idx_t low  = 0, high = (idx_t)x;
    idx_t ans  = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x) return mid;
        if (sq < x) { ans = mid; low = mid + 1; }
        else        { high = mid - 1; }
    }
    return ans;
}

/* smallest row r such that the nth triangular number ≥ n (1‑based rows) */
static inline idx_t row_of(idx_t n)
{
    /* initial bounds by doubling */
    idx_t lo = 0, hi = 1;
    while (( (__uint128_t)hi * (hi + 1) ) / 2 < n) hi <<= 1;
    /* binary search */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t tri = ( (__uint128_t)mid * (mid + 1) ) / 2;
        if (tri >= n) hi = mid;
        else          lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* only defined for n ≥ 1 */

    /* locate row r and column k */
    idx_t r = row_of(n);                /* 1‑based row index */
    __uint128_t before = ( (__uint128_t)(r - 1) * r ) / 2;  /* elements before row r */
    idx_t k = n - (idx_t)before;         /* column index within the row (1‑based) */

    if (r % k == 0) {                   /* k divides r */
        seq_t mu = mobius(r);           /* μ(r) ∈ {−1,0,1} */
        seq_t phi_k = phi(k);           /* φ(k) ≥ 0 */
        return mu * phi_k;
    }
    return 0;
}
