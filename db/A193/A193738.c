/*
 * A193738  Triangular array: the fusion of polynomial sequences P and Q given
 *          by p(n,x)=q(n,x)=x^n+x^(n-1)+...+x+1.
 * https://oeis.org/A193738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 for n=0; for n>0, let r be the largest integer * with
 *            r*(r+1)/2 <= n, and let j = n - r*(r+1)/2. * Then a(n) = r if j
 *            == r (last element of row r) * = j + 1 otherwise. * This
 *            corresponds to the triangular array with entries * min(j+1,r)
 *            for 0 <= j <= r
 * Range:     u8 n <= 5049, u16 n <= 5049, u32 n <= 5049, u64 n <= 5049,
 *            u128 n <= 5049, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t lo = 0;
    idx_t hi = n; /* hi may be large, binary search will converge quickly */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);           /* upper mid */
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t r = lo;

    if (r == 0)
        return (seq_t)1;

    /* Start index of row r: r*(r+1)/2 */
    __uint128_t start128 = (__uint128_t)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;
    idx_t j = n - start;          /* position within the row (0-based) */

    if (j == r)
        return (seq_t)r;
    else
        return (seq_t)(j + 1);
}
