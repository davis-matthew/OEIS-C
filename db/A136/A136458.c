/*
 * A136458  Triangle of coefficients of the characteristic polynomial of a
 *          bi-orthogonal n X n matrix: h(i,j) = If[i - j == 0, 1, If[Abs[i -
 *          j] - n/2 == 0, -1, 0]];i,j<=n; example n=4: {{1, 0, -1, 0}, {0, 1,
 *          0, -1}, {-1, 0, 1, 0}, {0, -1, 0, 1}}.
 * https://oeis.org/A136458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, tabl, sign
 * Formula:   a(n) = coefficient of the characteristic polynomial of the
 *            bi‑orthogonal n×n matrix (offset 1). For matrix size m = r‑1,
 *            the polynomial is m odd : (λ‑1)^m → coefficients c_j = (−1)^j *
 *            C(m, j) (descending order) m even : λ^ (m/2) (λ‑2)^(m/2) →
 *            coefficients c_j = C(m/2, m‑j) * (−2)^(m‑j) (ascendin...
 * Range:     i8 n <= 48, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>      /* for idx_t and seq_t, defined elsewhere */
#include <stdbool.h>     /* for bool, required by some build systems */

/* Compute binomial coefficient C(n, k) (0 ≤ k ≤ n), exactly. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        /* Multiply then divide; exact division guaranteed. */
        res *= (seq_t)(n - k + i);
        res /= (seq_t)i;
    }
    return res;
}

/* Return the n‑th term (1‑based) of the OEIS sequence A136458. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined, but safe */

    /* Find the smallest row r with T(r) = r*(r+1)/2 ≥ n. */
    idx_t r = 1;
    while ((unsigned __int128)r * (r + 1) / 2 < n)
        ++r;

    /* Previous total of terms: T(r‑1) = (r‑1)*r/2 */
    idx_t prev_terms = (r - 1) * r / 2;
    idx_t col = n - prev_terms - 1;            /* 0‑based column in row r */

    idx_t m = r - 1;                            /* matrix size */
    if (m == (idx_t)0)                          /* row r = 1 */
        return (seq_t)1;                        /* only one term, value 1 */

    if (m & 1) { /* m is odd : (λ‑1)^m, coefficients descending */
        seq_t bin = binom(m, col);
        if (col & 1) return -bin;
        return bin;
    }
    /* m is even : λ^(m/2) (λ‑2)^(m/2), coefficients ascending. */
    idx_t half = m / 2;                         /* m = 2*half */
    idx_t s = m - col;                          /* exponent of (λ‑2) part */
    if (s > half) return (seq_t)0;              /* coefficient 0 */

    seq_t coeff = binom(half, s);               /* binomial part */
    /* Multiply by 2^s */
    for (idx_t i = 0; i < s; ++i)
        coeff *= (seq_t)2;
    if (s & 1) coeff = -coeff;                  /* sign from (−2)^s */
    return coeff;
}
