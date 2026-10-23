/*
 * A014700  Poincaré series [or Poincare series] (or Molien series) for mod 2
 *          cohomology of universal W-group W(5).
 * https://oeis.org/A014700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Let c[0..15] =
 *            {1,5,40,176,440,835,1423,1980,1980,1423,835,440,176,40,5,1}.
 *            Then a(n) = Σ_{m=0}^{15} c[m] * C(14 + (n-m)/2 , (n-m)/2) ,
 *            where the summand is taken iff n ≥ m and n‑m is even. This
 *            follows from the generating function ( ∑_{m=0}^{15} c[m] t^m ) /
 *            (1–t²)^15
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 24, u64 n <= 149, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t binom_idx(idx_t n, idx_t k)
{
    /* Compute binomial(n,k) using only integer arithmetic on seq_t.
       Since 0 ≤ k ≤ n, the result fits into seq_t.          */
    if (k > n) return 0;
    if (k > n - k) k = n - k;       /* use symmetry           */
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (seq_t)(n - k + i);
        r = r / (seq_t)i;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Coefficients of the numerator polynomial. */
    static const idx_t c[16] = {
        1,5,40,176,440,835,1423,1980,1980,1423,835,440,176,40,5,1
    };

    seq_t result = 0;
    for (idx_t m = 0; m < 16; ++m) {
        if (n < m) break;                 /* further m give negative delta */
        idx_t delta = n - m;
        if ((delta & 1UL) != 0) continue; /* odd delta: no contribution    */
        idx_t k = delta >> 1;              /* δ = 2k                          */
        seq_t bin = binom_idx(14 + k, k);
        result += (seq_t)c[m] * bin;
    }
    return result;
}
