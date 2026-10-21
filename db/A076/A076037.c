/*
 * A076037  Square array read by antidiagonals in which row n has g.f.
 *          (1-(n-1)*x*C)/(1-n*x*C) where C = (1/2-1/2*(1-4*x)^(1/2))/x = g.f.
 *          for Catalan numbers A000108.
 * https://oeis.org/A076037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the antidiagonal entry of an infinite square array whose
 *            * row r has generating function G_r(x) = (1-(r-1)*x*C)/(1-r*x*C)
 *            * with C the Catalan generating function. * For n≥0 let * d =
 *            ⌊(√(8n+1)-1)/2⌋ (the diagonal index), * t = n - d*(d+1)/2
 *            (position within the diagonal), * i = d - t, j...
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t isqrt(idx_t v)
{
    idx_t low = 0, high = v, ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * mid <= v) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)(n - i + 1);
        res /= (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* diagonal index */
    idx_t v   = 8 * n + 1;
    idx_t d   = (isqrt(v) - 1) / 2;
    /* start position of this diagonal */
    idx_t start = d * (d + 1) / 2;
    /* position within the diagonal */
    idx_t t = n - start;
    /* row and column indices in the array */
    idx_t i = d - t;
    idx_t j = t;

    if (j == 0) return (seq_t)1;

    seq_t coeff = 0;
    seq_t power = 1;                /* i^(k-1) */
    for (idx_t k = 1; k <= j; ++k) {
        idx_t m = j - k;
        seq_t bin = binom_small(2*m + k, m);
        bin *= (seq_t)k;
        seq_t denom = 2*m + k;
        bin /= denom;               /* exact division */
        coeff += power * bin;
        power *= (seq_t)i;          /* update i^(k) for next iteration */
    }
    return coeff;
}
