/*
 * A187783  De Bruijn's triangle, T(m,n) = (m*n)!/(n!^m) read by downward
 *          antidiagonals.
 * https://oeis.org/A187783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(m,n) = (m*n)!/(n!)^m read by downward antidiagonals
 * Range:     u8 n <= 23, u16 n <= 31, u32 n <= 48, u64 n <= 71, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline idx_t diagonal_of(idx_t n)
/* returns the largest d such that d*(d+1)/2 <= n   (triangular test) */
{
    idx_t lo = 0;
    idx_t hi = (n == UINT64_MAX) ? UINT64_MAX : n * 2 + 1;
    idx_t best = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n) {
            best = mid;
            lo   = mid + 1;
        } else {
            if (mid == 0) break;
            hi   = mid - 1;
        }
    }
    return best;
}

static inline seq_t binom(idx_t N, idx_t K)
/* compute C(N,K) with multiplicative algorithm */
{
    if (K == 0 || K == N) return 1;
    idx_t k = K;
    if (k > N - k) k = N - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (N - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* locate (m,n) from index n by antidiagonal traversal */
    idx_t d = diagonal_of(n);                     /* diagonal number */
    unsigned __int128 offset128 = (unsigned __int128)d * (d + 1) / 2;
    idx_t offset = (idx_t)offset128;              /* triangular number */
    idx_t k = n - offset;                         /* position on diagonal */
    idx_t m = k;
    idx_t nn = d - k;                             /* n coordinate */

    /* special cases T(0,*) or T(*,0) are 1 */
    if (m == 0 || nn == 0) {
        return 1;
    }

    /* T(m,nn) = product_{i=1..m} C(i*nn, nn) */
    seq_t result = 1;
    for (idx_t i = 1; i <= m; ++i) {
        idx_t N = i * nn;
        result = result * binom(N, nn);
    }
    return result;
}
