/*
 * A232628  Denominators of the triangle of polynomial coefficients P(0,x)=1,
 *          2*P(n)=(1+x)*((1+x)^(n-1)+x^(n-1)).
 * https://oeis.org/A232628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A232628 – denominators of the triangle of polynomial
 * coefficients defined by
 *     P(0,x) = 1,
 *     2*P(n,x) = (1+x)*((1+x)^(n-1) + x^(n-1)).
 *
 * The function returns the denominator (1 or 2) of the coefficient
 * that appears at position n when the triangle is read row by row.
 *
 * Derived formula:
 *   For row r and column k (0‑based, n = r(r+1)/2 + k):
 *     if k == r              → denominator 1  (the last term of the row)
 *     else if k == r-1        → denominator 1 if r odd, else 2
 *     else                    → denominator 1 if C(r,k) is even,
 *                               otherwise 2   (C(r,k) odd ↔ (k & ~r) == 0)
 *
 * The routine works for any n that fits in idx_t and is efficient
 * (O(log n) due to a binary search for the row index).
 */

#include <stdint.h>

/* Find the largest r such that r*(r+1)/2 <= n.
 * Uses a binary search with 128‑bit arithmetic to avoid overflow. */
static inline idx_t row_of(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = n;           /* r can never exceed n */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Special case for the first coefficient */
    if (n == 0)
        return (seq_t)1;

    /* Determine the row r and column k */
    idx_t r = row_of(n);
    idx_t prev = (__uint128_t)r * (r + 1) / 2;
    idx_t k = n - prev;     /* 0‑based column within the row */

    /* Denominator logic as explained above */
    if (k == r)                     /* last element of the row */
        return (seq_t)1;

    if (k == r - 1) {
        /* second‑to‑last element: denom 1 if r is odd, else 2 */
        return ((r & 1) ? (seq_t)1 : (seq_t)2);
    }

    /* For the remaining elements, test parity of C(r,k) */
    return (((k & (~r)) == 0) ? (seq_t)2 : (seq_t)1);
}
