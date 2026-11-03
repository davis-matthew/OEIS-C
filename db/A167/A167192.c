/*
 * A167192  Triangle read by rows: T(n,k) = (n-k)/gcd(n,k), 1 <= k <= n.
 * https://oeis.org/A167192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the triangle T(m,k) = (m-k)/gcd(m,k),
 *            1 ≤ k ≤ m, read row‐wise. For n≥1: r = smallest integer with
 *            r(r+1)/2 ≥ n k = n - r(r-1)/2 a(n) = (r-k)/gcd(r,k)
 * Range:     u8 n <= 7260, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Validated: standard, pass, 118 terms through n = 6495 (2026-11-01)
 * Helpers:   gcd, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/gcd.h"
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but return 0 */
    /* find row r such that r(r+1)/2 ≥ n */
    idx_t s   = isqrt(8 * n + 1);
    idx_t r   = (s - 1) / 2;
    /* adjust if too small */
    if (r * (r + 1) / 2 < n) ++r;
    /* column index within the row */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;
    /* compute (r-k)/gcd(r,k) using seq_t arithmetic */
    seq_t sr = (seq_t)r;
    seq_t sk = (seq_t)k;
    seq_t diff = sr - sk;
    seq_t g = gcd(sr, sk);
    return diff / g;
}
