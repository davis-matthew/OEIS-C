/*
 * A223524  Triangle S(n, k) by rows: coefficients of
 *          2^(n/2)*(x^(1/2)*d/dx)^n, where n = 0, 2, 4, 6, ...
 * https://oeis.org/A223524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is recovered from the triangle S(r,k) where * S(r,k) =
 *            (2r)! / [(r-k)! * (2k)!] * 2^(2k - r). * * Index mapping: * r =
 *            floor((sqrt(8*n+1)-1)/2); * k = n - r*(r+1)/2;
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 27, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-01)
 * Helpers:   ipow_i32, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/pow2.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* --- map the flattened index to row r and column k --- */
    seq_t sq = ((seq_t)8 * (seq_t)n) + 1;           /* sq = 8*n + 1 */
    idx_t sqrt_val = (idx_t)floor_sqrt(sq);         /* floor(sqrt(8*n+1)) */
    idx_t r = (sqrt_val - 1) / 2;                   /* row */
    idx_t cum = r * (r + 1) / 2;                    /* terms before row r */
    idx_t k = n - cum;                              /* column in row r */

    /* --- compute S(r,k) = (2r)! / ((r-k)! * (2k)!) * 2^(2k - r) --- */
    idx_t two_r = 2 * r;
    idx_t two_k = 2 * k;

    seq_t num  = factorial(two_r);          /* (2r)!   */
    seq_t den1 = factorial(r - k);          /* (r-k)!  */
    seq_t den2 = factorial(two_k);          /* (2k)!   */

    seq_t ans = num / den1;
    ans = ans / den2;                       /* integer division, exact */

    int exp = (int)(2 * k) - (int)r;        /* exponent of 2 */
    if (exp >= 0) {
        ans *= pow2((idx_t)exp);
    } else {
        ans /= pow2((idx_t)(-exp));
    }

    return ans;
}
