/*
 * A195152  Square array read by antidiagonals with T(n,k) = n*((k+2)*n-k)/2,
 *          n=0, +- 1, +- 2,..., k>=0.
 * https://oeis.org/A195152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry read by antidiagonals from the array * with
 *            rows indexed 0,1,-1,2,-2,3,-3,... and columns k ≥ 0, * where the
 *            table entry is * T(m,k) = m * ((k+2) * m - k) / 2
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer.
   Returns floor(sqrt(x)). */
static inline idx_t sqrt128_floor(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1 << 64);
    while (lo + 1 < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    return (idx_t)lo;
}

/* Compute the sequence value a(n). */
seq_t a(idx_t n)
{
    /* 1. Find diagonal d such that S(d-1) <= n < S(d),
       where S(d) = (d+1)(d+2)/2.  Use the closed form
       via integer square root of 8*n+1. */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    idx_t sqrtv = sqrt128_floor(val);
    idx_t d = (sqrtv - 1) / 2;          /* diagonal number */

    /* 2. Compute offset j within the diagonal (0-based). */
    unsigned __int128 prevS = (unsigned __int128)d * (d + 1) / 2; /* S(d-1) */
    idx_t j = n - (idx_t)prevS;           /* 0 <= j <= d */

    /* 3. Convert diagonal index to row and column indices. */
    idx_t k = j;           /* column index */
    idx_t i = d - j;       /* row index in the permuted ordering */

    /* 4. Map row index i to actual n value:
           i=0 -> n=0
           i odd  -> n = (i+1)/2
           i even -> n = -(i/2) */
    int64_t m_n;          /* signed n for the table */
    if (i == 0) {
        m_n = 0;
    } else if (i & 1) {
        m_n = (int64_t)((i + 1) / 2);
    } else {
        m_n = - (int64_t)(i / 2);
    }

    /* 5. Evaluate T(m_n, k).  Use the equivalent formulas
       that avoid negative intermediates. */
    if (m_n >= 0) {
        seq_t nm = (seq_t)m_n;
        seq_t term1 = nm * nm;                                 /* n^2 */
        seq_t term2 = (seq_t)k * nm * (nm - 1) / 2;            /* k * n(n-1)/2 */
        return term1 + term2;
    } else {
        int64_t m = -(int64_t)m_n;                            /* m = -n > 0 */
        seq_t mm = (seq_t)m;
        seq_t term1 = mm * mm;                                 /* m^2 */
        seq_t term2 = (seq_t)k * mm * (mm + 1) / 2;            /* k * m(m+1)/2 */
        return term1 + term2;
    }
}
