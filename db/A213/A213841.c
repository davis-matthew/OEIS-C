/*
 * A213841  Rectangular array: (row n) = b**c, where b(h) = 2*h-1, c(h) =
 *          4*n-7+4*h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the (i,j)-th entry of the infinite array whose rows are
 *            the convolution of the odd numbers 1,3,5,… with the arithmetic
 *            progression 4*n-3,4*n+1,…; terms are taken in antidiagonal order
 * Range:     u8 n <= 15, u16 n <= 253, u32 n <= 253, u64 n <= 253,
 *            u128 n <= 253, big any n
 * Validated: standard, pass, 65 terms through n = 244 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static inline uint64_t find_diag(uint64_t n)
{
    /* Find smallest s with s*(s-1)/2 >= n.  Use double for a quick
       approximation and adjust.  Works for all n that fit in uint64_t. */
    uint64_t s = (uint64_t)(sqrt((double)n * 2.0) + 1.0);
    /* Adjust upward */
    while ((__int128)s * (s - 1) / 2 < n) ++s;
    /* Adjust downward (if overestimated) */
    while (s > 2 && (__int128)(s - 1) * (s - 2) / 2 >= n) --s;
    return s;
}

static inline seq_t T(idx_t r, idx_t k)
{
    /* Compute T(r,k) = (4r-3+4k)*k^2 - (4/3)*k*(k+1)*(2k+1) + 2*k*(k+1) */
    seq_t rk = (seq_t)r;
    seq_t kk = (seq_t)k;

    seq_t A = 4 * rk - 3 + 4 * kk;               /* 4r-3+4k    */
    seq_t term1 = A * kk * kk;                   /* (4r-3+4k)k^2 */
    seq_t term2 = (4 * kk * (kk + 1) * (2 * kk + 1)) / 3; /* (4/3)*k(k+1)(2k+1) */
    seq_t term3 = 2 * kk * (kk + 1);             /* 2k(k+1)    */
    return term1 - term2 + term3;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* out‑of‑range value, undefined for OEIS */
    uint64_t s = find_diag((uint64_t)n);
    uint64_t totalPrev = (uint64_t)(((__int128)(s - 1) * (s - 2)) / 2);
    uint64_t pos = n - totalPrev; /* 1‑based position on diagonal s */
    uint64_t row = pos;           /* i */
    uint64_t col = s - pos;       /* j = s-i */
    return T(row, col);
}
