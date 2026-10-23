/*
 * A168621  Triangle read by rows: T(n,0) = T(n,n) = 1 for n >= 0, T(n,k) =
 *          ((n - 1)! + 1)*binomial(n, k) for 1 <= k <= n - 1, n >= 2.
 * https://oeis.org/A168621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 if n corresponds to a triangle edge; otherwise a(n) =
 *            ((r−1)! + 1) * binomial(r, k) where r is the unique row index
 *            with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 and k = n − r*(r+1)/2
 * Range:     u8 n <= 21, u16 n <= 37, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static inline idx_t
_find_row(idx_t n)
{
    idx_t lo = 0, hi = 4294967295ULL; /* sufficient for all 64‑bit n */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        /* use 128‑bit to avoid overflow */
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t r   = _find_row(n);
    __int128 start128 = (__int128)r * (r + 1) / 2;
    idx_t   start   = (idx_t)start128;
    idx_t   k       = n - start;                 /* position in row */

    /* edges of the triangle */
    if (k == 0 || k == r || r < 2) {
        return 1;
    }

    /* compute (r-1)! */
    seq_t factorial = 1;
    for (idx_t i = 2; i <= r - 1; ++i) {
        factorial = factorial * (seq_t)i;
    }

    seq_t term = (factorial + 1) * binomial(r, k);
    return term;
}
