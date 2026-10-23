/*
 * A137277  Triangle of the coefficients [x^k] P_n(x) of the polynomials
 *          P_n(x) = 1/n * sum(j=0..floor(n/2), (-1)^j * binomial(n,j) *
 *          (n-4*j) * x^(n-2*j) ).
 * https://oeis.org/A137277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = coefficient of x^k in P_r(x), where r is the row such
 *            that n is the (n‑th) term in the flattened triangle of
 *            coefficients of the polynomials P_r(x) = 1/r * sum_{j=0}^{⌊r/2⌋}
 *            (-1)^j * binomial(r,j) * (r-4*j) * x^(r-2*j). The flattened
 *            sequence lists the coefficients of each row in order of...
 * Range:     i8 n <= 28, i16 n <= 92, i32 n <= 92, i64 n <= 92, i128 n <= 92,
 *            big any n
 * Helpers:   binomial, prefix
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/prefix.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* P_0(x) = 1, its only coefficient is 1 */

    /* Find the row r that contains the n‑th flattened element. */
    idx_t low = 0;
    idx_t high = 1;
    while (prefix(high) <= n)
        high <<= 1;               /* grow high until prefix(high) > n */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (prefix(mid) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low;                /* first row with prefix(r) > n, so n lies in row r */

    idx_t prev = (r == 0) ? 0 : prefix(r - 1);   /* start index of row r */
    idx_t k = n - prev;             /* exponent of x in row r (0‑based) */

    /* Coefficient equals zero when (r - k) is odd. */
    if (((r - k) & 1) != 0)
        return (seq_t)0;

    idx_t j = (r - k) / 2;          /* summation index */
    seq_t bin = binomial(r, j);     /* C(r, j) */
    seq_t term = (seq_t)r - (seq_t)(4 * j);   /* (r - 4*j) */
    seq_t prod = bin * term;        /* (-1)^j * bin * (r-4*j) */
    if (j & 1)
        prod = -prod;               /* apply (-1)^j */

    /* Division by r is exact. */
    return prod / (seq_t)r;
}
