/*
 * A137289  Triangle read by rows: T(n,k) =
 *          (-1)^(n-k)*(C(k+n-1,n-k)-2*C(k+n-1,n-k-1)) for n>=0 and 0<=k<=n.
 * https://oeis.org/A137289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   Let m = n − 1 (zero‑based index in the flattened triangle). Find
 *            the row r and column k such that r·(r+1)/2 ≤ m < (r+1)·(r+2)/2,
 *            k = m − r·(r+1)/2, then a(n) = (−1)^(r−k) · ( C(k+r−1 , r−k) −
 *            2·C(k+r−1 , r−k−1) ). The triangle is read by rows starting with
 *            offset 1
 * Range:     i8 n <= 24, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = 4294967296ULL;   /* √(2^64−1) = 2^32 */
    while (low < high) {
        idx_t mid = (low + high) / 2;
        if (mid == 0) break;
        if (mid > x / mid)
            high = mid - 1;
        else
            low = (mid + 1 <= high) ? mid + 1 : mid;
    }
    return low - 1;
}

/* Computes binomial coefficient n choose k using multiplicative formula. */
static inline seq_t my_binomial(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(n - k + i));
        res = res / ((seq_t)i);
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range, safe default */

    idx_t m = n - 1;                      /* zero‑based index in triangle */
    /* Compute row r: r = floor((sqrt(8m+1)-1)/2) */
    idx_t t   = 8 * m + 1;
    idx_t sq  = isqrt_u64(t);
    idx_t r   = (sq - 1) / 2;

    /* Adjust r in case of rounding errors */
    while ((r + 1) * (r + 2) / 2 <= m) r++;
    while (r * (r + 1) / 2 >  m)          r--;

    idx_t k = m - r * (r + 1) / 2;

    if (r == 0) return (seq_t)-1;          /* first row */

    seq_t bin1 = my_binomial(k + r - 1, r - k);
    seq_t bin2 = (r > k) ? my_binomial(k + r - 1, r - k - 1) : (seq_t)0;

    seq_t val = bin1 - (seq_t)2 * bin2;
    if ((r - k) & 1) val = -val;
    return val;
}
