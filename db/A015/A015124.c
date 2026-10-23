/*
 * A015124  Triangle of q-binomial coefficients for q=-11.
 * https://oeis.org/A015124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 23, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   gcd, pow_q, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Gaussian q-binomial coefficient
 *   a(n) = [r choose k]_q  with q = -11,
 *   where (r,k) are the row and column corresponding to the
 *   n-th entry of the triangular array read row by row.
 *
 * The n-th entry is obtained by fixing the unique row r such that
 *   r*(r+1)/2 <= n < (r+1)*(r+2)/2
 * and k = n - r*(r+1)/2.
 *
 * The Gaussian binomial coefficient is computed multiplicatively:
 *   [r choose k]_q = product_{i=1..k} (1 - q^{r-i+1}) / (1 - q^i),
 *   using exact division with gcd reductions to keep intermediate
 *   values within range.  For arbitrary‑precision seq_t the
 *   operations are exact.  For 64‑bit seq_t the function
 *   still works for values that fit in 64 bits, as required by
 *   the tests.
 */

#include <stdint.h>
#include "../utils/seq_gcd.h"     /* seq_gcd(seq_t a, seq_t b) */
#include "../utils/gcd.h"         /* gcd(seq_t a, seq_t b) */
#include "../utils/pow_q.h"

static inline idx_t row_of(idx_t idx) {
    /* find r such that r*(r+1)/2 <= idx < (r+1)*(r+2)/2 */
    idx_t low = 0, high = 1;
    /* increase high until start(high) > idx */
    while ((high * (high + 1)) / 2 <= idx) high <<= 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((mid * (mid + 1)) / 2 <= idx) low = mid;
        else high = mid - 1;
    }
    return low;
}

/* multiplicative computation of the Gaussian binomial coefficient */
static seq_t q_binom(idx_t n, idx_t k, seq_t q) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    if (k == 0) return 1;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* numerator: 1 - q^(n-i+1) */
        seq_t num = 1 - pow_q(q, n - i + 1);
        /* denominator: 1 - q^i */
        seq_t den = 1 - pow_q(q, i);

        /* reduce any common factors to keep intermediate values small */
        seq_t g1 = seq_gcd(result, den);
        if (g1 != 0) { result /= g1; den /= g1; }

        seq_t g2 = seq_gcd(num, den);
        if (g2 != 0) { num /= g2; den /= g2; }

        seq_t g3 = seq_gcd(num, result);
        if (g3 != 0) { num /= g3; result /= g3; }

        result = result * num;   /* den should now be 1 */
    }
    return result;
}

seq_t a(idx_t n) {
    const seq_t Q = -11;         /* fixed q value */

    idx_t r = row_of(n);
    idx_t k = n - (r * (r + 1)) / 2;

    return q_binom(r, k, Q);
}
