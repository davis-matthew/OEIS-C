/*
 * A146773  Triangle read by rows: expansion of p(x,n)=If[n == 0, 1, (x + 1)^n
 *          + 2^(n)*Sum[Binomial[n-m, m]*x^m*(1 + x^(n - 2*m)), {m, 1, n -
 *          1}]].
 * https://oeis.org/A146773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) is the n-th entry of the triangle read row‑wise, where row
 *            r contains the coefficients of the polynomial p(x,r) = (x+1)^r +
 *            2^r * Σ_{m=1}^{r-1} binomial(r-m,m) * x^m * (1 + x^{r-2m})
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper to compute binomial(n,k) using a multiplicative formula.
   Works for all integer types that support the required arithmetic. */
static seq_t binomial_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = (result * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return result;
}

/* Helper to compute 2^exp in seq_t, avoiding overflow by repeated
   multiplication (works for big‑int builds; for 64‑bit builds it
   overflows naturally but those values are outside the test range). */
static seq_t pow2_big(idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= 2;
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* Position k within row r. */
    idx_t k = n - r * (r + 1) / 2;

    /* Start with contribution from (x+1)^r. */
    seq_t coeff = binomial_small(r, k);

    /* Add contributions from the sum part. */
    if (r >= 2) { /* sum range is m=1..r-1; for r<2 it is empty. */
        seq_t pow2r = pow2_big(r);
        for (idx_t m = 1; m < r; ++m) {
            seq_t term = pow2r * binomial_small(r - m, m);
            if (k == m)          coeff += term;
            if (k == r - m)      coeff += term; /* doubles when m==r-m */
        }
    }

    return coeff;
}
