/*
 * A213211  Triangular array read by rows: T(n,k) is the number of size k
 *          subsets of {1,2,...,n} such that (when the elements are arranged
 *          in increasing order) the smallest element is congruent to 1 mod 3
 *          and the difference of every pair of successive elements is also
 *          congruent to 1 mod 3.
 * https://oeis.org/A213211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(k + floor((r - k)/3), k) where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r*(r+1)/2 (i.e. read the
 *            triangular array T(r,k) with T(n,k) = C(k + floor((n - k)/3), k)
 *            by rows)
 * Range:     u8 n <= 196, u16 n <= 607, u32 n <= 2104, u64 n <= 7673,
 *            u128 n <= 9558, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: integer square root for unsigned __int128 */
static idx_t
isqrt_u128(unsigned __int128 x)
{
    idx_t lo = 0;
    idx_t hi = ((idx_t)1 << 34);   /* safe upper bound: sqrt(8*2^64+1) < 2^34 */
    idx_t ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break; /* avoid underflow */
            hi = mid - 1;
        }
    }
    return ans;
}

/* Helper: binomial coefficient, computed multiplicatively.  */
static seq_t
binomial_seq(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;   /* use symmetry to reduce loop */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        unsigned __int128 prod = (unsigned __int128)res * (n - i + 1);
        res = (seq_t)(prod / i);
    }
    return res;
}

seq_t a(idx_t n)
{
    /* r = floor((sqrt(8*n+1)-1)/2)  */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    idx_t r = isqrt_u128(val);
    r = (r - 1) >> 1;   /* (r-1)/2 */

    /* k = n - r*(r+1)/2  */
    unsigned __int128 prev_mid = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)prev_mid;

    /* m = k + floor((r - k)/3)  */
    idx_t m = k + (r - k) / 3;

    return binomial_seq(m, k);
}
