/*
 * A193897  Triangular array: the self-fusion of (p(n,x)), where
 *          p(n,x)=sum{(k+1)*x^k : 0<=k<=n}.
 * https://oeis.org/A193897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Let r be the unique integer such that r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2, and let k = n – r(r+1)/2. Then a(n) = r(r+1)/2 if
 *            k == r (last element of row r) a(n) = binom(k+2,2) · (r – k + 1)
 *            if k < r with binom(k+2,2) = (k+2)(k+1)/2. Special case: a(0)=1
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute triangular number T(r) = r(r+1)/2  as a 128‑bit integer */
static inline __int128 tri128(idx_t r)
{
    return (__int128)r * (r + 1) / 2;
}

seq_t a(idx_t n)
{
    /* handle the very first term */
    if (n == 0) return 1;

    /* binary search for r: triangular(r) ≤ n < triangular(r+1) */
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        __int128 t = tri128((__int128)mid);
        if (t <= n)  lo = mid;
        else         hi = mid - 1;
    }
    idx_t r = lo;                            /* row number */
    idx_t offset = (idx_t)tri128((__int128)r);
    idx_t k = n - offset;                    /* column number in that row */

    if (k == r) {                            /* last column */
        return (seq_t)tri128((__int128)r);   /* binom(r+1,2) = r(r+1)/2 */
    } else {                                 /* inner columns */
        __int128 bin = (__int128)(k + 2) * (k + 1) / 2;   /* binom(k+2,2) */
        __int128 result = bin * (__int128)(r - k + 1);
        return (seq_t)result;
    }
}
