/*
 * A054515  Number of ways to place non-intersecting diagonals in convex
 *          (n+2)-gon so as to create no quadrilaterals.
 * https://oeis.org/A054515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/(n+1) * Σ_{k=0..⌊n/3⌋} binomial(n+k, k) *
 *            binomial(2n-k, n-3k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative binomial coefficient: C(n,k) with seq_t precision. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;           // use symmetry
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i); // multiply by numerator
        res = res / (seq_t)i;           // divide by denominator (exact)
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t maxk = n / 3;                 /* floor(n/3) */
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t bin1 = binom(n + k, k);           /* C(n+k, k) */
        seq_t bin2 = binom(2 * n - k, n - 3 * k); /* C(2n-k, n-3k) */
        sum = sum + bin1 * bin2;
    }
    return sum / (seq_t)(n + 1);  /* divide by (n+1), exact by theory */
}
