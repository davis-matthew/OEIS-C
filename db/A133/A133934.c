/*
 * A133934  A007318 * (a diagonalized version of A124625).
 * https://oeis.org/A133934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A133934
 * Triangle entries: T(r,c) = C(r-1, c-1) * diag[c]
 *  where diag[c] = 1 if c is odd, otherwise c.
 * Row sums are A129954.
 *
 * a(n) is the n-th element of the triangle read row‐wise (row major).
 * For n ≥ 1:
 *   Find r such that T(r-1) < n ≤ T(r)  where T(k) = k(k+1)/2.
 *   Let j = n - T(r-1).  (1 ≤ j ≤ r)
 *   a(n) = C(r-1, j-1) * ((j%2==0)?j:1)
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r with triangular number T(r) ≥ n */
    idx_t lo = 1, hi = n;                 /* hi may be larger than needed but safe */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 m = (unsigned __int128)mid;
        unsigned __int128 tmid = m * (m + 1) / 2;
        if (tmid >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;                          /* r satisfies T(r-1) < n ≤ T(r) */

    /* Compute previous triangular number */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t j = n - (idx_t)prev;              /* column index (1‑based) */

    /* binomial coefficient C(r-1, j-1) */
    idx_t N = r - 1;
    idx_t K = j - 1;
    if (K > N - K)   /* use symmetry to reduce computations */
        K = N - K;
    seq_t binom = 1;
    for (idx_t i = 1; i <= K; ++i) {
        binom = binom * (seq_t)(N - K + i) / (seq_t)i;
    }

    /* diagonal value */
    seq_t diag = (j & 1ULL) ? (seq_t)1 : (seq_t)j;

    return binom * diag;
}
