/*
 * A099514  Triangle, read by rows, of trinomial coefficients arranged so that
 *          there are n+1 terms in row n by setting T(n,k) equal to the
 *          coefficient of z^k in (1 + z + 2*z^2)^(n-[k/2]), for n>=k>=0,
 *          where [k/2] is the integer floor of k/2.
 * https://oeis.org/A099514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of z^k in (1 + z + 2z²)^(r – ⌊k/2⌋), *
 *            where r is the unique integer with r(r+1)/2 ≤ n < (r+1)(r+2)/2,
 *            * and k = n – r(r+1)/2. This is the n‑th element of the triangle
 *            * read by rows
 * Range:     u8 n <= 40, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r and column k such that
     *     r(r+1)/2 ≤ n < (r+1)(r+2)/2
     *   and  k = n – r(r+1)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t k = n - r * (r + 1) / 2;

    /* Exponent of the base polynomial. */
    idx_t e = r - k / 2;

    /* j ranges over all representations i + 2j = k with i ≤ e − j. */
    idx_t lower = 0;
    if (k > e) {
        lower = k - e;     /* j must be at least k−e. */
    }
    idx_t upper = k / 2;   /* highest possible j. */

    seq_t sum = 0;
    seq_t pow2 = 1;        /* 2^j, updated incrementally. */
    for (idx_t j = 0; j <= upper; ++j) {
        if (j > 0) {
            pow2 *= (seq_t)2;   /* multiply by 2 each step. */
        }
        if (j < lower) {
            continue;          /* j too small. */
        }
        idx_t i = k - 2 * j;    /* i = k - 2j. */
        seq_t term = binomial(e, j);
        term *= binomial(e - j, i);
        term *= pow2;
        sum += term;
    }
    return sum;
}
