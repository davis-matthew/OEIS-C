/*
 * A118745  Triangle of coefficients of polynomials giving the n-anacci
 *          constants.
 * https://oeis.org/A118745
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) = coefficient of x^k in the polynomial P_r(x) = -1 +
 *            2(r-1)x + Σ_{k=2}^{r-1} (r-2-k)x^k where r ≥ 2 is the unique
 *            integer such that the first (n-2) terms of the triangle lie in
 *            rows 2 … r-1. Equivalently, if t = n-1 and we define S(r) =
 *            r(r+1)/2, then r is the smallest integer with S(r) > t....
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;              /* below stated offset – undefined */

    /* Convert to zero‑based index within the flattened triangle */
    idx_t idx = n - 2;
    idx_t t   = idx + 1;              /* t = n-1 used in the quadratic formula */

    /* Find the row r: smallest integer with r(r+1)/2 > t  */
    idx_t r = 2;
    while (((unsigned __int128)r * (r + 1) / 2) <= t)
        ++r;

    /* Number of terms in all preceding rows: Σ_{i=2}^{r-1} i */
    idx_t prev_cnt = ((unsigned __int128)(r - 1) * r / 2) - 1;

    /* Position (0‑based) inside row r */
    idx_t k = idx - prev_cnt;

    /* Compute the coefficient using a wider signed type to avoid UB */
    int64_t val;
    if (k == 0) {
        val = -1;
    } else if (k == 1) {
        val = (int64_t)2 * (int64_t)(r - 1);
    } else {
        val = (int64_t)(r - 2 - k);
    }

    return (seq_t)val;
}
