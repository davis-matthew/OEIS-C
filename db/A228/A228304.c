/*
 * A228304  a(n) = Sum_{k=0..n} C(n,k)^4*(-1)^k.
 * https://oeis.org/A228304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=0..n} (-1)^k · C(n,k)^4 (C(n,k) denotes the binomial
 *            coefficient). Odd‑indexed terms are identically zero. The
 *            implementation computes the binomial coefficients iteratively,
 *            avoiding factorials or tableaux
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 11, i64 n <= 19, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 1, odd n → 0 */
    if (n == 0)
        return (seq_t)1;
    if ((n & 1) == 1)
        return (seq_t)0;

    seq_t sum   = 0;
    seq_t binom = 1;          /* C(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {
            /* binom = binom * (n - k + 1) / k */
            seq_t num   = (seq_t)(n - k + 1);
            binom = (binom * num) / (seq_t)k;
        }

        /* term = binom^4  (kept in two squarings) */
        seq_t term = binom * binom;   /* binom^2 */
        term = term * term;           /* binom^4 */

        if (k & 1)
            sum -= term;
        else
            sum += term;
    }

    return sum;
}
