/*
 * A177375  Triangle t(n,k): the coefficient [x^k] of the series (1+x)^n +
 *          2*n*x*(1+x)^(n-2), in row n, column k.
 * https://oeis.org/A177375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient [x^k] of (1+x)^r + 2*r*x*(1+x)^(r-2) where r
 *            and k satisfy r*(r+1)/2 <= n < (r+1)*(r+2)/2 and k = n -
 *            r*(r+1)/2. The sequence is obtained by enumerating the triangle
 *            row‑by‑row from left to right
 * Range:     u8 n <= 38, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;          /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find row r such that (r+1)(r+2)/2 > n and r*(r+1)/2 <= n. */
    idx_t lo = 0, hi = n, r = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t cum = (__uint128_t)(mid + 1) * (mid + 2) / 2;
        if (cum > n) {
            r = mid;
            if (mid == 0) break;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    /* Compute column index k. */
    idx_t prev = r * (r + 1) / 2;      /* number of terms before row r */
    idx_t k = n - prev;

    /* Special cases for small rows. */
    if (r == 0) return (seq_t)1;
    if (r == 1) return (k == 0) ? (seq_t)1 : (seq_t)3;

    /* General case: t(r,k) = C(r,k) + 2*r*C(r-2,k-1). */
    seq_t term1 = binom_calc(r, k);
    seq_t term2 = 0;
    if (k >= 1) {
        term2 = binom_calc(r - 2, k - 1);
        term2 = term2 * (seq_t)(2 * r);
    }
    return term1 + term2;
}
