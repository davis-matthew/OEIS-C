/*
 * A050478  a(n) = C(n)*(8*n+1) where C(n) = Catalan numbers (A000108).
 * https://oeis.org/A050478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binom(2*n, n) + 7*binom(2*n, n-1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* multiplicative binomial coefficient C(n,k) with reduction to avoid overflow */
static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);   /* numerator factor */
        seq_t den = (seq_t)i;              /* denominator factor */

        /* cancel common factors between num and den */
        seq_t g = gcd(num, den);
        num /= g; den /= g;

        /* cancel common factors between res and remaining den */
        g = gcd(res, den);
        res /= g; den /= g;

        /* den should now be 1; multiply the reduced num */
        res = res * num;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    idx_t two_n = n * (idx_t)2;
    seq_t c1 = _binom(two_n, n);
    seq_t c2 = _binom(two_n, n - (idx_t)1);

    return c1 + (seq_t)7 * c2;
}
