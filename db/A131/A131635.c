/*
 * A131635  Triangle T(n,m)=m*n*binomial(m+n,m)^2/(2*(m+n)) read by rows.
 * https://oeis.org/A131635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(m,n) where T(m,n) = m*n*binomial(m+n,m)^2/(2*(m+n)) The
 *            sequence is obtained by reading the symmetric triangle T(m,n)
 *            row‑by‑row, with a(1) = T(1,1)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 35, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    if (n == 0)    /* out of range (offset 1) */
        return 0;

    /* Find the row number R such that R(R+1)/2 ≥ n and (R-1)R/2 < n. */
    idx_t low  = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    idx_t R = low;                     /* row index (second triangle index) */

    /* Number of terms before this row. */
    idx_t prev = (R - 1) * R / 2;
    /* Column index within the row (first triangle index). */
    idx_t C = n - prev;                /* 1 .. R */

    idx_t m = C, k = R;                /* triangle indices: T(m,k) */

    seq_t bin   = binomial(m + k, m);  /* binomial(m+k,m) */
    seq_t bin_sq = bin * bin;          /* square of the binomial */
    seq_t mk = (seq_t)m * (seq_t)k;     /* product m*k in seq_t */
    seq_t numer = mk * bin_sq;         /* m*k*bin^2 */
    seq_t denom = (seq_t)(2 * (m + k));/* 2*(m+k) */

    return numer / denom;
}
