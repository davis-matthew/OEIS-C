/*
 * A136672  Riordan array ((1+x^2)/(1-x)^2, -x/(1-x)^2).
 * https://oeis.org/A136672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is the nth entry of the Riordan array ((1+x²)/(1‑x)²,
 *            –x/(1‑x)²) read row by row. For the row index r (0‑based) and
 *            the column index k (0‑based) we have m = r – k a = binom(2k + m
 *            + 1, m) + (m ≥ 2 ? binom(2k + m – 1, m – 2) : 0) a(n) = (k even
 *            ? +a : –a). The mapping from n to (r,k) is r = s...
 * Range:     i8 n <= 17, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find row r (0‑based) that contains the nth term. */
    idx_t r = 0;
    while ((r + 1U) * (r + 2U) / 2U < n)
        ++r;

    /* Column index k within that row (0‑based). */
    idx_t k = n - r * (r + 1U) / 2U - 1U;

    /* m = r – k. */
    idx_t m = r - k;

    /* Compute the binomial terms. */
    seq_t val = binom(2U * k + m + 1U, m);
    if (m >= 2U)
        val += binom(2U * k + m - 1U, m - 2U);

    /* Multiply by (–1)^k. */
    if (k & 1U)
        val = -val;

    return val;
}
