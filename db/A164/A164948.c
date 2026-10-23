/*
 * A164948  Fibonacci matrix read by antidiagonals. (Inverse of A136158.)
 * https://oeis.org/A164948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = T(r,c) where r = max k such that k(k+1)/2 ≤ n, c = n -
 *            r(r+1)/2, and T(r,c) = (-1)^c*C(r-1,c)*3^(r-1-c) -
 *            (-1)^(c-1)*C(r-1,c-1)*3^(r-c), with T(0,0)=1
 * Range:     i8 n <= 15, i16 n <= 46, i32 n <= 172, i64 n <= 600,
 *            i128 n <= 1325, big any n
 * Helpers:   binom, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use binomial and pow3 utilities from ../utils. */

#include "../utils/binom.h"
#include "../utils/pow3.h"

static idx_t row_from_index(idx_t n)
{
    /* binary search for r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t lo = 0, hi = (idx_t)1ULL << 32;   /* safe upper bound for uint64_t indices */
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* determine (r,c) from linear index n */
    idx_t r = row_from_index(n);
    __int128 tri128 = (__int128)r * (r + 1) / 2;
    idx_t t = (idx_t)tri128;          /* t = r(r+1)/2 */
    idx_t c = n - t;                  /* column index */

    if (r == 0)
        return (seq_t)1;              /* T(0,0) */

    /* Compute first term: (-1)^c * C(r-1,c) * 3^(r-1-c) */
    seq_t term1 = 0;
    if (c <= r - 1)                     /* otherwise binom = 0, ignore */
    {
        seq_t bin1 = binom(r - 1, c);
        seq_t pow1 = pow3((idx_t)(r - 1 - c));
        term1 = bin1 * pow1;
        if (c & 1)                      /* multiply by (-1)^c */
            term1 = -term1;
    }

    /* Compute second term: (-1)^(c-1) * C(r-1,c-1) * 3^(r-c) */
    seq_t term2 = 0;
    if (c >= 1)                          /* otherwise binom = 0, ignore */
    {
        seq_t bin2 = binom(r - 1, c - 1);
        seq_t pow2 = pow3((idx_t)(r - c));
        term2 = bin2 * pow2;
        if ((c - 1) & 1)                 /* multiply by (-1)^(c-1) */
            term2 = -term2;
    }

    /* Desired value is term1 - term2 */
    return term1 - term2;
}
