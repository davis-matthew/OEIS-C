/*
 * A156820  T(n,m) = Sum_{j=0..m} (-1)^(j + m)*(j + 1)^n*binomial(m, j) +
 *          Sum_{j=0..(n-m)} (-1)^(j - m + n )*(1 + j)^n*binomial(n-m, j).
 * https://oeis.org/A156820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where (r,c) are the row‑column of the flattened
 *            index n (row‑major order) and T(n,m) = Sum_{j=0..m} (-1)^{j+m}
 *            (j+1)^n * binomial(m,j) + Sum_{j=0..(n-m)} (-1)^{j-m+n} (1+j)^n
 *            * binomial(n-m,j)
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

/* integer square‑root for 64‑bit unsigned integers */
static idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (idx_t)1ULL << 32;   /* sqrt(2^64‑1) < 2^32 */
    idx_t res = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * (__uint128_t)mid;
        if (sq <= x) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* determine (row,rowcol) from flattened index n */
    idx_t row = (floor_sqrt_u64(8 * n + 1) - 1) / 2;
    idx_t start = row * (row + 1) / 2;
    idx_t col = n - start;

    seq_t result = 0;

    /* first sum: j = 0 .. col */
    for (idx_t j = 0; j <= col; ++j) {
        seq_t term = seq_pow((seq_t)(j + 1), row);
        term *= binomial(col, j);
        if ((j + col) & 1) /* odd exponent → negative sign */
            result -= term;
        else
            result += term;
    }

    /* second sum: j = 0 .. (row-col) */
    idx_t maxj = row - col;
    for (idx_t j = 0; j <= maxj; ++j) {
        seq_t term = seq_pow((seq_t)(j + 1), row);
        term *= binomial(maxj, j);
        if ((j + row - col) & 1)
            result -= term;
        else
            result += term;
    }

    return result;
}
