/*
 * A035869  Coordination sequence for A_43 lattice.
 * https://oeis.org/A035869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0; a(n) = Σ_{d=1}^{43} C(44,d) * C(n-1,d-1) *
 *            C(43-d+n,n) for n ≥ 1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* helper to compute small binomial coefficients with k ≤ 43
   (k is the smaller of the two parameters) */
static seq_t binom_small(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;            /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;     /* exact division */
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    idx_t k = n;                          /* m/2 = n */
    seq_t sum = 0;
    for (idx_t d = 1; d <= 43; ++d) {
        seq_t term   = binom_small(44, d);
        term *= binom_small(k - 1, d - 1);
        term *= binom_small(43 - d + k, k);
        sum += term;
    }
    return sum;
}
