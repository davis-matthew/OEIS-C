/*
 * A026000  a(n) = T(2n, n), where T is the Delannoy triangle (A008288).
 * https://oeis.org/A026000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(2n, n) = Σ_{k=0}^{n} binom(2n, k) · binom(n, k) · 2^k
 *            where T is the Delannoy triangle (A008288). This implementation
 *            computes the sum multiplicatively, keeping all intermediate
 *            values within the bounds of seq_t. No hard‑coded values are used
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = 2 * n  (as seq_t) */
    seq_t m = (seq_t)2 * n;

    seq_t sum   = 0;
    seq_t C2    = 1; /* binom(2n, 0) */
    seq_t C1    = 1; /* binom(n, 0)  */
    seq_t pow2  = 1; /* 2^0          */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = C2 * C1;
        term = term * pow2;
        sum += term;

        if (k == n) break;

        /* Update binomial coefficients and 2^k for next k */
        seq_t den = (seq_t)(k + 1);
        seq_t num2 = m - (seq_t)k;           /* 2n - k   */
        seq_t num1 = (seq_t)n - (seq_t)k;    /* n  - k   */

        C2 = (C2 * num2) / den;
        C1 = (C1 * num1) / den;
        pow2 <<= 1;
    }

    return sum;
}
