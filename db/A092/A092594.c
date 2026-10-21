/*
 * A092594  Triangle read by rows: T(n,k) is the number of permutations p of
 *          [n] in which the length of the longest initial segment avoiding
 *          both the 132- and the 231-pattern is equal to k.
 * https://oeis.org/A092594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth entry of the triangle T(n,k) * T(n,k) =
 *            (k-1)*n!*2^(k-1)/(k+1)! for k < n * = 2^(n-1) for k = n * This
 *            code flattens that triangle row by row
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factorial.h"
#include "../utils/pow2.h"

/* integer square root: floor(sqrt(x)) for non‑negative x */
static idx_t
isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1u) >> 1;
        if (mid > 0u && mid > x / mid) hi = mid - 1u;
        else                         lo = mid;
    }
    return lo;
}

seq_t
a(idx_t n)
{
    if (n == 0u) return 0u;          /* undefined for n = 0, return 0 */

    /* find row r such that r*(r+1)/2 >= n  (triangle number) */
    idx_t r = (isqrt_u64(8u * n + 1u) - 1u) >> 1;
    if ((r * (r + 1u)) / 2u < n) ++r;

    /* column index within the row (1‑based) */
    idx_t prev_sum = (r - 1u) * r / 2u;
    idx_t c = n - prev_sum;          /* 1 <= c <= r */

    /* T(r,r) = 2^(r-1) */
    if (c == r) return pow2(c - 1u);

    /* T(r,1) = 0 */
    if (c == 1u) return 0u;

    /* T(r,c) = (c-1)*r!*2^(c-1)/(c+1)!  (c < r) */
    seq_t val = factorial(r);                /* r!   */
    val *= (seq_t)(c - 1u);                  /* *(c-1) */
    val *= pow2(c - 1u);                     /* *2^(c-1) */
    val /= factorial(c + 1u);                /* / (c+1)! */
    return val;
}
