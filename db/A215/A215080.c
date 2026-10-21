/*
 * A215080  T(n,k) = Sum_{j=0..k} (k-j)^n * binomial(n,j).
 * https://oeis.org/A215080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8n+1)-1)/2⌋, k = n - r(r+1)/2 and
 *            T(r,k) = Σ_{j=0..k} (k−j)^r * binomial(r,j) (triangle edges are
 *            enumerated row‑major)
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 62, u64 n <= 134,
 *            u128 n <= 374, big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

/* Find the largest r such that r(r+1)/2 <= n. */
static inline idx_t
row_from_index(idx_t n)
{
    idx_t lo = 0, hi = n;           /* hi is an upper bound */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __int128 tri = (__int128)mid * (mid + 1ULL) / 2;
        if (tri <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)                   /* T(0,0) = 1 */
        return (seq_t)1;

    idx_t r = row_from_index(n);                 /* row index   */
    idx_t tri_r = (idx_t)(((__int128)r * (r + 1ULL)) / 2);
    idx_t k = n - tri_r;             /* column index */

    seq_t sum = 0;
    for (idx_t j = 0; j <= k; ++j) {
        seq_t pow_val = seq_pow((seq_t)(k - j), r);
        seq_t bin_val = binomial(r, j);
        sum = sum + pow_val * bin_val;
    }
    return sum;
}
