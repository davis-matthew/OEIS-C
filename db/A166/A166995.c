/*
 * A166995  G.f.: C(x) = Sum_{n>=0} log(1 - 2^(2n)*x)^(2n)/(2n)!, a power
 *          series in x with integer coefficients.
 * https://oeis.org/A166995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( C(2^n + n - 1, n) + (-1)^n * C(2^n, n) ) / 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Compute C(N, k) where N is a seq_t (possibly huge) and 0 <= k <= n. */
static seq_t binomial_big(seq_t N, idx_t k) {
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < k; ++i) {
        seq_t numerator = N - (seq_t)i;   /* N - i */
        result = result * numerator;
        result = result / (seq_t)(i + 1);  /* exact division */
    }
    return result;
}

seq_t a(idx_t n) {
    /* Handle n = 0 directly (avoids using pow2 of 0 slightly faster). */
    if (n == 0) return (seq_t)1;

    seq_t pow2_n = pow2(n);                     /* 2^n */
    seq_t N1      = pow2_n + (seq_t)n - (seq_t)1; /* 2^n + n - 1 */

    seq_t term1 = binomial_big(N1, n);          /* C(2^n + n - 1, n) */
    seq_t term2 = binomial_big(pow2_n, n);      /* C(2^n, n) */

    if (n & 1U)  /* n odd: subtract */
        term1 = term1 - term2;
    else         /* n even: add */
        term1 = term1 + term2;

    term1 = term1 / (seq_t)2;                   /* divide by 2 */
    return term1;
}
