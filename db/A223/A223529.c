/*
 * A223529  Triangle S(n,k) by rows: coefficients of
 *          5^((n-1)/2)*(x^(1/5)*d/dx)^n when n=1,3,5,...
 * https://oeis.org/A223529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For the flattened index n ≥ 1, let r be the unique row such that
 *            r(r+1)/2 ≥ n > (r‑1)r/2, and let k = n – (r‑1)r/2 (the column
 *            inside that row, 1 ≤ k ≤ r). The term a(n) is given by a(n) =
 *            5^(k‑1) * C(r‑1, k‑1) * ∏_{j=k+1}^{r} (5j – 4), where C(n, k) is
 *            the binomial coefficient. The product is ta...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 ≥ n > (r‑1)r/2.  */
    idx_t low = 1, high = n;   /* high is safely an upper bound */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* use __int128 to avoid overflow when computing mid*(mid+1)/2 */
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;
    __int128 triPrev = (__int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)triPrev;

    /* 5^(k‑1) */
    seq_t pow5 = 1;
    for (idx_t i = 1; i < k; ++i)
        pow5 *= (seq_t)5;

    /* binomial C(r‑1, k‑1) */
    idx_t m   = r - 1;
    idx_t kk  = k - 1;
    if (kk > m - kk)          /* use the smaller of kk and m‑kk */
        kk = m - kk;
    seq_t binom = 1;
    for (idx_t i = 1; i <= kk; ++i) {
        binom *= (seq_t)(m - kk + i);
        binom /= (idx_t)i;    /* exact division */
    }

    /* ∏_{j=k+1}^{r} (5j‑4)  (empty product → 1 when k==r) */
    seq_t prod = 1;
    for (idx_t j = k + 1; j <= r; ++j)
        prod *= ((seq_t)5 * j - 4);

    /* assemble the result */
    seq_t result = pow5 * binom;
    result *= prod;
    return result;
}
