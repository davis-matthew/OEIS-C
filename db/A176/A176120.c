/*
 * A176120  Triangle read by rows: Sum_{j=0..k} binomial(n, j)*binomial(k,
 *          j)*j!.
 * https://oeis.org/A176120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = Σ_{j=0}^{k} binom(r, j) * binom(k, j) * j! * where r is
 *            the unique non‑negative integer satisfying * r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2 * and k = n - r(r+1)/2. This yields the triangle
 *            T(r, k) * from the OEIS entry A176120, flattened row‑by‑row
 * Range:     u8 n <= 18, u16 n <= 34, u32 n <= 88, u64 n <= 208,
 *            u128 n <= 559, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ---------- small helper: binomial coefficient --------------------------------
 * Computes C(n, k) for arbitrary-precision seq_t.  The algorithm is
 * multiplicative and only uses exact integer arithmetic.
 */
static seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        result = result * (seq_t)(n - k + i);
        result = result / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t k = n - r * (r + 1) / 2;

    /* Compute T(r, k) = Σ_{j=0}^{k} C(r, j) * C(k, j) * j! */
    seq_t sum   = 0;
    seq_t fact  = 1;          /* j! for current j */
    for (idx_t j = 0; j <= k; ++j)
    {
        seq_t term = binom_calc(r, j);
        term = term * binom_calc(k, j);
        term = term * fact;
        sum += term;
        fact *= (seq_t)(j + 1);   /* prepare (j+1)! for next iteration */
    }
    return sum;
}
