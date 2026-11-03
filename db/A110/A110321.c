/*
 * A110321  A Jacobsthal number related number triangle.
 * https://oeis.org/A110321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) with r = row_of(n) and k = n - r*(r+1)/2 T(r,k) =
 *            r! * J(r - k + 1) / k! where J(m) is the Jacobsthal sequence:
 *            J(0)=0, J(1)=1, J(m)=J(m-1)+2*J(m-2)
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 48 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include the utility that provides row_of */
#include <stdint.h>

static inline idx_t
row_of(idx_t n)                 /* return r with r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
{
    idx_t low  = 0;
    idx_t high = 1;
    /* exponential search to find an upper bound */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        low  = high;
        high <<= 1;              /* double high */
    }
    /* binary search between low (inclusive) and high (inclusive) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1) + 1; /* upper mid to avoid infinite loop */
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Compute Jacobsthal number J(m) for 0 <= m <= ~200 */
static seq_t jacobsthal(idx_t m)
{
    if (m == 0)
        return 0;
    if (m == 1)
        return 1;

    seq_t j0 = 0;            /* J(0) */
    seq_t j1 = 1;            /* J(1) */
    seq_t js;
    for (idx_t i = 2; i <= m; ++i) {
        js = j1 + (seq_t)2 * j0;   /* J(i) = J(i-1) + 2*J(i-2) */
        j0 = j1;
        j1 = js;
    }
    return js;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine row r and column k */
    idx_t r = row_of(n);                               /* 0‑based row */
    idx_t k = n - r * (r + 1) / 2;                     /* 0‑based column */

    /* Compute product r! / k!  (product of (k+1) .. r) */
    seq_t prod = 1;
    for (idx_t i = k + 1; i <= r; ++i)
        prod *= (seq_t)i;

    /* Jacobsthal index: r - k + 1 */
    idx_t jacob_idx = r - k + 1;
    seq_t j = jacobsthal(jacob_idx);

    return prod * j;
}
