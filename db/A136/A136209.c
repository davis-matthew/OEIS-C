/*
 * A136209  Differentiation of A137286: Triangle of coefficients of
 *          differentiation recursive orthogonal Hermite polynomials given in
 *          Hochstadt's book : P(x, n) = x*P(x, n - 1) - n*P(x, n - 2).
 * https://oeis.org/A136209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, tabl, sign
 * Formula:   a(n) = j * coeff_j(P_r) where P_0(x) = 1, P_1(x) = x, P_k(x) = x
 *            * P_{k-1}(x) - k * P_{k-2}(x) (k ≥ 2), and r, j satisfy n =
 *            r*(r-1)/2 + j with 1 ≤ j ≤ r
 * Range:     i8 n <= 16, i16 n <= 55, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* find row r and column j (1‑based) such that n = r*(r-1)/2 + j */
    idx_t r = 1;
    idx_t cum = 1;
    while (cum < n) {
        ++r;
        cum += r;
    }
    idx_t j = n - (r - 1) * r / 2;   /* 1 ≤ j ≤ r */

    /* allocate coefficient arrays for polynomials up to degree r */
    idx_t len = r + 1;               /* indices 0 … r  */
    seq_t *prev2 = alloc_seq_array(len);  /* P_{k-2} */
    seq_t *prev1 = alloc_seq_array(len);  /* P_{k-1} */
    if (!prev2 || !prev1)
        return (seq_t)0;            /* allocation failed */

    /* initialise P_0 */
    for (idx_t i = 0; i < len; ++i) prev2[i] = (seq_t)0;
    prev2[0] = (seq_t)1;

    /* initialise P_1 */
    for (idx_t i = 0; i < len; ++i) prev1[i] = (seq_t)0;
    if (r >= 1) prev1[1] = (seq_t)1;

    /* build polynomials up to P_r */
    for (idx_t k = 2; k <= r; ++k) {
        seq_t *curr = alloc_seq_array(len);   /* P_k */
        if (!curr) {
            seq_free_array(prev2);
            seq_free_array(prev1);
            return (seq_t)0;
        }
        for (idx_t d = 0; d <= k; ++d) {
            seq_t val = (seq_t)0;
            if (d > 0) val += prev1[d - 1];                /* x * P_{k-1} */
            if (d <= k - 2) val -= (seq_t)k * prev2[d];    /* - k * P_{k-2} */
            curr[d] = val;
        }
        seq_free_array(prev2);
        prev2 = prev1;
        prev1 = curr;
    }

    /* a(n) = j * coeff_j(P_r) */
    seq_t coeff = prev1[j];          /* coefficient of x^j in P_r */
    seq_t result = (seq_t)j * coeff;

    seq_free_array(prev2);
    seq_free_array(prev1);
    return result;
}
