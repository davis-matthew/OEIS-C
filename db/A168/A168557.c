/*
 * A168557  Triangle T(n,k) read by rows: coefficient [x^k] of the polynomial
 *          (-1)^n*((x + 1)^n - x^n + 1), 0 <= k <= max(0, n - 1).
 * https://oeis.org/A168557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, easy
 * Formula:   T(n,k) = (-1)^n*binomial(n,k) + (-1)^n*delta(0,k) - delta(0,n)
 * Range:     i8 n <= 31, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is obtained by flattening the triangle T by rows. */
#include "../utils/binomial.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find the triangle row r such that 1 + r(r+1)/2 > n */
    idx_t r = 0;
    while (1 + (r * (r + 1)) / 2 <= n)
        r++;

    /* Starting index of row r (number of elements before this row) */
    idx_t start = (r == 0) ? 0 : 1 + ((r - 1) * r) / 2;

    /* Column index within the row */
    idx_t k = n - start;

    /* Sign factor (-1)^r */
    seq_t sign = (r & 1) ? (seq_t)-1 : (seq_t)1;

    /* Compute T(r,k) using the provided binomial helper */
    seq_t val = sign * binomial(r, k);

    /* Add the delta(0,k) term for k == 0 */
    if (k == 0)
        val += sign;

    /* Subtract the delta(0,r) term for r == 0 */
    if (r == 0)
        val -= 1;

    return val;
}
