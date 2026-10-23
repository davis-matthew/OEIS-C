/*
 * A035862  Coordination sequence for A_36 lattice.
 * https://oeis.org/A035862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d=1..36} C(37,d) * C(n-1,d-1) * C(36-d+n, n) (m=2n)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility: binomial coefficient C(n,k) with small k (k <= 35) */
#include "../utils/gcd.h"

static seq_t binom_uint(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry to keep k small */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i);   /* numerator part */
        seq_t den = (seq_t)i;             /* denominator part */

        seq_t g1 = gcd(num, den);         /* cancel common factors */
        num /= g1;
        den /= g1;

        seq_t g2 = gcd(res, den);
        res /= g2;
        den /= g2;

        res *= num;                       /* multiply */
        res /= den;                       /* divide (exact) */
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;                /* offset 0 */

    seq_t sum = 0;
    for (idx_t d = 1; d <= 36; ++d)
    {
        seq_t term = binom_uint(37, d);
        seq_t t1   = binom_uint(n - 1, d - 1);
        if (!t1) continue;               /* zero product, skip */
        seq_t t2   = binom_uint(36 - d + n, n);
        term *= t1;
        term *= t2;
        sum += term;
    }
    return sum;
}
