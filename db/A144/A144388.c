/*
 * A144388  Triangle T(n,k) = binomial(n, k) + ((-1)^(n + k))*n*binomial(n -
 *          1, k), T(0,0) = 1, read by rows, 0 <= k <= n.
 * https://oeis.org/A144388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = T(row, col) where * T(r, k) = binomial(r, k) + (-1)^(r+k)
 *            * r * binomial(r-1, k) * row and col are obtained from the
 *            flattened triangle: * row = max r such that r*(r+1)/2 <= n <
 *            (r+1)*(r+2)/2, * col = n - r*(r+1)/2 (0 ≤ col ≤ r) * a(0) = 1
 * Range:     i8 n <= 30, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) with idx_t indices */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

/* Return the n‑th term of OEIS sequence A144388 */
seq_t a(idx_t n)
{
    /* Find the row r: r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* Column index within the row */
    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;

    /* First part of T(r,c) */
    seq_t term = binom_idx(r, c);

    /* Second part, if r > 0 */
    if (r > 0) {
        seq_t add = (seq_t)r * binom_idx(r - 1, c);
        if (((r + c) & 1) == 0)      /* even exponent -> + */
            term += add;
        else                         /* odd exponent  -> - */
            term -= add;
    }
    return term;
}
