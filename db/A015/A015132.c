/*
 * A015132  Triangle of (Gaussian) q-binomial coefficients for q=-14.
 * https://oeis.org/A015132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 10, i32 n <= 22, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A015132: Triangle of (Gaussian) q‑binomial coefficients for q = -14,
   read by antidiagonals.  For n >= 0,
   find the diagonal d such that T(d‑1) <= n < T(d) where
   T(d) = (d+1)(d+2)/2.  Then let r = d - (n - T(d‑1)),
   and return the binomial coefficient C_{-14}(d,r).  */

/* Forward declaration of helper */
static seq_t q_binom(idx_t n, idx_t k);

seq_t a(idx_t n)
{
    /* Term 0 is 1 by definition */
    if (n == 0)
        return 1;

    /* Find diagonal d via small linear search (d <= sqrt(2*n)) */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;

    /* Number of terms before this diagonal */
    idx_t prev = d * (d + 1) / 2;

    /* Position on diagonal (0..d) */
    idx_t pos  = n - prev;

    /* Corresponding column index r = d - pos */
    idx_t r = d - pos;

    return q_binom(d, r);
}

/* Compute Gaussian q‑binomial coefficient C_q(n,k) at q = -14 */
static seq_t q_binom(idx_t n, idx_t k)
{
    if (k == 0 || k == n)
        return 1;

    /* Use symmetry to minimise loops */
    if (k > n - k)
        k = n - k;

    /* Precompute powers of q = -14 up to n */
    seq_t *pow = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!pow) {
        /* Handle allocation failure gracefully */
        return 0;
    }
    pow[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        pow[i] = pow[i - 1] * (seq_t)(-14);

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = 1 - pow[n + 1 - i];
        seq_t den = 1 - pow[i];
        res = res * num / den;   /* Exact division by construction */
    }

    free(pow);
    return res;
}
