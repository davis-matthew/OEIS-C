/*
 * A015117  Triangle of q-binomial coefficients for q=-7.
 * https://oeis.org/A015117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 16, i32 n <= 30, i64 n <= 58, i128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A015117: Triangle of q‑binomial coefficients for q = -7
   a(n) is the nth entry of the triangle read by antidiagonals.
   The triangle entry T[n,k] is the Gaussian binomial coefficient
   \binom{n}{k}_{q} with q = -7, and a(n) is obtained by
   mapping n to the position (r,k) with 0 ≤ k ≤ r and
   r(r+1)/2 ≤ n < (r+1)(r+2)/2.  */

#include <stdlib.h>

/* Euclidean gcd for seq_t.  Works for any signed integer type. */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    if (a < (seq_t)0) a = -a;
    if (b < (seq_t)0) b = -b;
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Gaussian binomial coefficient [n choose k]_{q=-7} */
static seq_t gaussian_binom(idx_t n, idx_t k)
{
    if (k == (idx_t)0 || k == n) return (seq_t)1;

    /* Use symmetry to minimise k */
    if (k > n - k) k = n - k;

    const seq_t q = (seq_t)-7;

    /* Pre‑compute powers q^i for i=0..n */
    seq_t *qpow = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!qpow) return (seq_t)0; /* allocation failure → 0 */

    qpow[0] = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i)
        qpow[i] = qpow[i - 1] * q;

    seq_t num = (seq_t)1;
    seq_t den = (seq_t)1;

    for (idx_t i = (idx_t)1; i <= k; ++i) {
        seq_t num_term = (seq_t)1 - qpow[n + 1 - i];
        seq_t den_term = (seq_t)1 - qpow[i];

        /* Cancel common factors between num_term and den */
        seq_t g1 = seq_gcd(num_term, den);
        num_term /= g1;
        den     /= g1;

        /* Cancel common factors between den_term and num */
        seq_t g2 = seq_gcd(den_term, num);
        den_term /= g2;
        num     /= g2;

        num *= num_term;
        den *= den_term;
    }

    /* Final simplification: ensure den divides num */
    seq_t g = seq_gcd(num, den);
    num /= g;
    den /= g;

    free(qpow);
    return num / den;
}

/* Public entry point required by the build */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = (idx_t)0;
    while (((r * (r + (idx_t)1)) / (idx_t)2) <= n)
        ++r;
    --r;

    idx_t start_of_row = (r * (r + (idx_t)1)) / (idx_t)2;
    idx_t k = n - start_of_row;          /* position within the row */

    return gaussian_binom(r, k);
}
