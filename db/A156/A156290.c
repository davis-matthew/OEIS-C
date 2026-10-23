/*
 * A156290  Triangle read by rows: alternating binomial coefficients with
 *          signs.
 * https://oeis.org/A156290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = read the triangle row‑wise where R(k,j) = (-1)^(k+j) *
 *            binomial(2k, k+j) (1 ≤ j ≤ k) k is the smallest integer with
 *            k*(k+1)/2 ≥ n j = n – k*(k-1)/2
 * Range:     i8 n <= 6, i16 n <= 28, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   _row_from_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_row_from_n.h"

/* multiplicative binomial coefficient: binomial(n, k) */
static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* res = res * (n - k + i) / i   (exact division) */
        seq_t num = (seq_t)(n - k + i);
        res = res * num;
        seq_t den = (seq_t)i;
        res = res / den;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0; /* out of range */

    idx_t k = _row_from_n(n);
    idx_t prev = (k - (idx_t)1) * k / 2;   /* terms before this row */
    idx_t j = n - prev;                   /* column within the row (1‑based) */

    seq_t bin = _binom(2 * k, k + j);
    seq_t sign = ((k + j) & (idx_t)1) ? (seq_t)(-1) : (seq_t)1;
    return sign * bin;
}
