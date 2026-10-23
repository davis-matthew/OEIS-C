/*
 * A123218  Irregular triangle formed by coefficients of polynomials defined
 *          by P(n,k,x) = f(n,k)*(2*x)^k*(1 - x^2)^(n - k), where f(n, k) =
 *          (-1)^floor((k + 1)/2)* binomial(n - floor((k + 1)/2), floor(k/2)).
 * https://oeis.org/A123218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, sign
 * Formula:   a(n) is the coefficient of x^(n-(r-1)^2-1) in the polynomial
 *            Q(m)(x) = Σ_{k=0}^m f(m,k)*(2x)^k*(1-x^2)^(m-k), where
 *            f(m,k)=(-1)^⌊(k+1)/2⌋·C(m-⌊(k+1)/2⌋,⌊k/2⌋)
 * Range:     i8 n <= 40, i16 n <= 110, i32 n <= 418, i64 n <= 1559,
 *            i128 n <= 6005, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t binom_small(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for 0, safe default */

    /* find r such that r^2 >= n > (r-1)^2 */
    idx_t r = 0;
    while ((r * r) < n) r++;

    idx_t m = r - 1;                          /* polynomial index */
    idx_t prev_sq = (r - 1) * (r - 1);
    idx_t j = n - prev_sq;                    /* 1-based position in row */
    idx_t d = j - 1;                          /* power of x, 0-based */

    seq_t ans = 0;

    for (idx_t k = 0; k <= m; ++k) {
        if (d < k) continue;
        idx_t diff = d - k;
        if ((diff & 1) != 0) continue;        /* d - k must be even */
        idx_t t = diff >> 1;
        if (t > m - k) continue;

        /* f(m,k) */
        idx_t floor_k1_2 = (k + 1) >> 1;
        idx_t b_half_k = k >> 1;
        seq_t bin_f = binom_small(m - floor_k1_2, b_half_k);
        int sign_f = ((floor_k1_2 & 1) == 0) ? 1 : -1;

        /* binomial(m-k, t) */
        seq_t bin_k_t = binom_small(m - k, t);

        /* 2^k */
        seq_t pow2_k = ((seq_t)1) << k;

        seq_t term = bin_f;
        term = term * pow2_k;
        term = term * bin_k_t;

        if (sign_f < 0) term = -term;
        if ((t & 1) != 0) term = -term;

        ans += term;
    }

    return ans;
}
