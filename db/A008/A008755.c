/*
 * A008755  Expansion of (1+x^12)/((1-x)*(1-x^2)*(1-x^3)).
 * https://oeis.org/A008755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6*n^2 - 36*n + 263 + 9*(-1)^n + 16*(-1)^n*cos(n*π/3)) /
 *            36 for n ≥ 7 For n < 7: a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=4,
 *            a(5)=5, a(6)=7
 * Range:     u8 n <= 41, u16 n <= 630, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Precomputed values for n = 0..6 */
    static const seq_t small[7] = {1, 1, 2, 3, 4, 5, 7};

    if (n < 7) return small[n];

    /* Compute numerator of the closed‑form expression */
    /* All intermediate arithmetic is done with signed 64‑bit integers.
       The values of n involved (≤ 10⁴ in the required tests) keep the
       results far below the 64‑bit limits. */
    int64_t nn = (int64_t)n;
    int64_t sq = nn * nn;                 /* n² */
    int64_t term1 = 6 * sq;
    int64_t term2 = -36 * nn;
    int64_t term3 = 263;

    /* (-1)^n = +1 for even n, -1 for odd n */
    int64_t sgn = (n & 1ULL) ? -1 : 1;

    int64_t term4 = 9 * sgn;              /* 9*(-1)^n */

    /* 16*(-1)^n*cos(n*π/3) takes only two distinct values:
       +16 when n ≡ 0 or 3 (mod 6), otherwise -8. */
    int64_t term5 = (((n % 6ULL) == 0ULL) || ((n % 6ULL) == 3ULL)) ? 16 : -8;

    int64_t numerator = term1 + term2 + term3 + term4 + term5;

    /* Division by 36 yields an integer for all n ≥ 7 */
    return (seq_t)(numerator / 36);
}
