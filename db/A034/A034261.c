/*
 * A034261  Infinite square array f(a,b) = C(a+b,b+1)*(a*b+a+1)/(b+2), a, b >=
 *          0, read by antidiagonals. Equivalently, triangular array T(n,k) =
 *          f(k,n-k), 0 <= k <= n, read by rows.
 * https://oeis.org/A034261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = f(k, n-k) where f(a,b)=C(a+b,b+1)*(a*b+a+1)/(b+2) (k = n
 *            - T(r), r = floor( (sqrt(1+8n)-1)/2 ), T(r)=r(r+1)/2)
 * Range:     u8 n <= 40, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <stdint.h>

/* Integer binary search for the largest r such that r*(r+1)/2 <= n */
static idx_t row_for_n(idx_t n)
{
    idx_t lo = 0, hi = ((idx_t)1 << 32);   /* sufficient for all n up to 2^64-1 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;     /* upper mid to avoid infinite loop */
        unsigned __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* find  row r and index k on that row */
    idx_t r = row_for_n(n);                                /* 0‑based row index    */
    unsigned __int128 pref = (__int128)r * (r + 1) / 2;      /* elements before row r */
    idx_t k = n - (idx_t)pref;                              /* column index (0‑based) */
    idx_t b = r - k;                                        /* b = r - k */

    /* compute f(k,b) */
    seq_t bin = binomial(r, b + 1);                         /* C(r, b+1)            */
    seq_t mult = (seq_t)k * (seq_t)b + (seq_t)k + (seq_t)1; /* (k*b + k + 1)        */
    seq_t denom = (seq_t)(b + 2);                           /* (b+2)                */

    return bin * mult / denom;
}
