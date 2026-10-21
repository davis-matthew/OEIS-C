/*
 * A089464  Hyperbinomial transform of A089461. Also the row sums of triangle
 *          A089463, which lists the coefficients for the third hyperbinomial
 *          transform.
 * https://oeis.org/A089464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3*(n-k+3)^(n-k-1) * C(n, k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (n-k) is treated as 0≤m≤n;  when m=0 the factor (m+3)^(m-1)=3^(-1) is understood
   so that the term equals 1.  The implementation computes the sum by iterating
   over m = n-k and using an incremental binomial coefficient.                 */

#include <stdint.h>

/* binomial(n, k) and seq_pow are not needed; the whole computation is done
   with elementary integer arithmetic.  The function works for any seq_t
   that supports the operators +, -, *, / and for any idx_t that is at least
   64‑bit unsigned. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                /* a(0) = 1 */

    /* term for m = 0 (i.e. k = n) contributes 1 */
    seq_t sum = (seq_t)1;

    /* C(n, 0) = 1, will be updated for each subsequent m */
    seq_t binom = (seq_t)1;

    for (idx_t m = 1; m <= n; ++m) {
        /* binomial coefficient C(n, m) = C(n, m-1) * (n-m+1) / m */
        binom = (binom * (n - m + 1)) / m;

        /* compute (m+3)^(m-1) by repeated multiplication */
        seq_t pow = (seq_t)1;
        idx_t exp = m - 1;
        for (idx_t i = 0; i < exp; ++i)
            pow *= (seq_t)(m + 3);

        /* term = 3 * C(n, m) * (m+3)^(m-1) */
        seq_t term = binom * (seq_t)3 * pow;

        sum += term;
    }

    return sum;
}
