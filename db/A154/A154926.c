/*
 * A154926  Signed version of Pascal's triangle. Diagonal positive, rest
 *          negative.
 * https://oeis.org/A154926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = C(r-1, c-1) if n = r(r+1)/2 (diagonal entry) * a(n) =
 *            -C(r-1, c-1) otherwise (off‑diagonal entry) * where r = smallest
 *            integer such that r(r+1)/2 ≥ n, * c = n - r(r-1)/2 (column
 *            index, 1‑based)
 * Range:     i8 n <= 39, i16 n <= 45, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, void * if needed */

static seq_t binom_multiplicative(idx_t n, idx_t k)
{
    if (k > n) return 0;               /* zero if k>n */
    if (k == 0 || k == n) return 1;    /* trivial cases */

    /* take advantage of symmetry */
    if (k > n - k) k = n - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply before dividing; division is exact */
        result = result * (seq_t)(n - k + i);
        result = result / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row r such that T(r-1) < n <= T(r), where T(r)=r(r+1)/2 */
    idx_t r = 1;
    idx_t t = 1;          /* T(r) for current r */
    while (n > t) {
        ++r;
        t += r;           /* T(r) = T(r-1)+r */
    }

    /* Compute the column */
    idx_t t_prev = t - r;                 /* T(r-1) */
    idx_t c = n - t_prev;                 /* 1‑based column index */
    idx_t m = r - 1;                      /* row index for binomial */
    idx_t k = c - 1;                      /* column index for binomial */

    seq_t coeff = binom_multiplicative(m, k);
    if (c != r)                   /* off‑diagonal entries are negative */
        coeff = -coeff;
    return coeff;
}
