/*
 * A158286  Triangle T(n, k) = coefficients of p(n, x), where p(n, x) =
 *          (-1)^n*(1+x)*((n+1)^2 +x)^(n-1), p(0, x) = 1, and p(1, x) = -1-x,
 *          read by rows.
 * https://oeis.org/A158286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, less
 * Formula:   a(n) = coefficient of x^k in (-1)^r*(1+x)*((r+1)^2 + x)^(r-1), *
 *            where r is the row index found from (r+1)(r+2)/2 > n and * k = n
 *            - r(r+1)/2 is the column index
 * Range:     i8 n <= 5, i16 n <= 14, i32 n <= 27, i64 n <= 65, i128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Exponentiation by squaring for seq_t. */
static inline seq_t power_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    seq_t cur     = base;
    while (exp > 0) {
        if (exp & 1U)
            result = result * cur;
        cur = cur * cur;
        exp >>= 1;
    }
    return result;
}

/* Multiplicative binomial coefficient: C(n,k). */
static seq_t binom_idx(idx_t n, idx_t k) {
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    /* Special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Determine row r and column k. */
    idx_t r   = 0;
    idx_t cum = 1; /* (r+1)(r+2)/2 for r = 0 */
    while (cum <= n) {
        ++r;
        cum = (r + 1) * (r + 2) / 2;
    }
    idx_t prev = r * (r + 1) / 2;
    idx_t k    = n - prev;          /* column index */

    /* Sign factor (-1)^r. */
    seq_t sign = (seq_t)((r & 1U) ? -1 : 1);

    /* Base (r+1)^2. */
    seq_t A = (seq_t)(r + 1) * (r + 1);

    if (k == r) {            /* last element of the row */
        return sign;
    } else if (k == 0) {     /* first element of the row */
        seq_t val = power_seq(A, r - 1);
        return sign * val;
    } else {                 /* middle coefficients */
        seq_t bin1 = binom_idx(r - 1, k);
        seq_t bin2 = binom_idx(r - 1, k - 1);
        seq_t pow1 = power_seq(A, (r - 1) - k);
        seq_t pow2 = power_seq(A, r - k);
        seq_t val  = bin1 * pow1 + bin2 * pow2;
        return sign * val;
    }
}
