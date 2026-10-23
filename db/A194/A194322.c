/*
 * A194322  Triangular array: g(n,k)=number of fractional parts (i*sqrt(1/2))
 *          in interval [(k-1)/n, k/n], for 1<=i<=2n, 1<=k<=n.
 * https://oeis.org/A194322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) for OEIS A194322.
   The sequence is the triangular array g(n,k) flattened row by row.
   The entry g(n,k) is the number of indices 1 ≤ i ≤ 2n such that
   the fractional part of i*sqrt(1/2) lies in [(k-1)/n , k/n).
   The computation uses only integer arithmetic.
   For each index n the function finds the row r and column k and
   counts the qualifying i by evaluating floor(i*sqrt(2)) and
   floor(i*n*sqrt(2)) via an integer square‑root routine. */

#include <stdint.h>

static uint64_t isqrt128(__int128 x)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 63) - 1;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __int128 mid2 = (__int128)mid * mid;
        if (mid2 == x) return mid;
        if (mid2 < x) lo = mid + 1;
        else           hi = mid - 1;
    }
    return hi;
}

static uint64_t floor_sqrt2(uint64_t n)
{
    /* floor(n*sqrt(2)) = floor(sqrt(2*n^2)) */
    __int128 m = (__int128)2 * n * n;
    return isqrt128(m);
}

static seq_t g_value(idx_t r, idx_t k)
{
    seq_t cnt = 0;
    for (idx_t i = 1; i <= 2 * r; ++i) {
        uint64_t A = floor_sqrt2(i);                 /* floor(i*sqrt(2)) */
        uint64_t C = r * (A - (A & 1ULL));           /* = r*(floor(A/2)*2)     */
        uint64_t D = i * r;                          /* argument for D*sqrt(2) */
        uint64_t S = floor_sqrt2(D);                 /* floor(D*sqrt(2))       */
        uint64_t low  = C + 2 * (k - 1);
        uint64_t high = C + 2 * k;
        if (S >= low && S < high) ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used, offset is 1 */
    idx_t r = 1;
    idx_t sum = 1;          /* triangular number r*(r+1)/2 */
    while (sum < n) {
        ++r;
        sum += r;
    }
    idx_t k = n - (sum - r);   /* column in row r (1‑based) */
    return g_value(r, k);
}
