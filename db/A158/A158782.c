/*
 * A158782  Irregular triangle of coefficients of p(n, x) = (1 -
 *          x^2)^(n+1)*Sum_{j >= 0} (4*j+ 1)^n*x^(2*j), read by rows.
 * https://oeis.org/A158782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the coefficient of x^n in p(n,x) = (1 - x^2)^(n+1) *
 *            \sum_{j>=0} (4*j+1)^n * x^(2*j). The coefficient can be computed
 *            as [x^n] p(n,x) = 0 if n is odd [x^(2m)] p(n,x) =
 *            \sum_{t=0}^{min(n+1,m)} (-1)^t * C(n+1,t) * (4*(m-t)+1)^n where
 *            C(a,b) is the binomial coefficient
 * Range:     u8 n <= 17, u16 n <= 39, u32 n <= 86, u64 n <= 234,
 *            u128 n <= 644, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* inexpensive floor sqrt for uint64_t, avoiding overflow */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t r = (idx_t) sqrt((double) n);
    /* adjust downwards if needed */
    while (r > n / r) --r;
    /* adjust upwards if needed */
    while ((r + 1) <= n / (r + 1)) ++r;
    return r;
}

/* fast exponentiation for seq_t base^exp, exp fits in idx_t */
static seq_t pow_seq_u64(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row r and column k in the triangle.
       offset_row(r) = r^2. */
    idx_t r = floor_sqrt_u64(n);
    idx_t k = n - r * r;           /* column index within the row (0-based) */
    if (k & 1ULL)                  /* odd indices are always zero */
        return (seq_t)0;

    /* m is the half‑exponent (since coefficient index k = 2*m). */
    idx_t m = k >> 1;              /* integer division by 2 */

    /* Summation as described above. */
    idx_t max_t = (r + 1 < m ? r + 1 : m);
    seq_t pos = 0, neg = 0;        /* separate positive and negative parts */
    seq_t binom = 1;               /* C(r+1, 0) */

    for (idx_t t = 0; t <= max_t; ++t)
    {
        seq_t base = (seq_t)(4 * (m - t) + 1);
        seq_t term = pow_seq_u64(base, r);
        term *= binom;            /* multiply by the current binomial coefficient */

        if ((t & 1U) == 0)
            pos += term;          /* positive contribution */
        else
            neg += term;          /* negative contribution */

        /* Update binomial coefficient for next t:
           C(r+1, t+1) = C(r+1, t) * (r+1 - t) / (t+1) */
        if (t < max_t)
        {
            seq_t numer = (seq_t)(r + 1 - t);
            seq_t denom = (seq_t)(t + 1);
            binom = (binom * numer) / denom;
        }
    }

    return pos - neg;
}
