/*
 * A098401  a(n) = (0^n + 3^n*binomial(2*n,n))/2.
 * https://oeis.org/A098401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3^n * binomial(2n,n))/2 Recurrence for n ≥ 2: a(n) =
 *            6*(2*n-1)/n * a(n-1) Initial values: a(0) = 1, a(1) = 3. The
 *            implementation uses the recurrence but reduces intermediate
 *            products with gcd to avoid overflow in intermediate values
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* handle the first two terms directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;

    seq_t res = (seq_t)3;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* multiplier 6*(2*i-1) */
        seq_t mult = (seq_t)(6 * (2 * i - 1));

        /* reduce the product res*mult by the denominator i safely */
        idx_t denom = i;

        /* first cancel common factors between res and the denominator */
        seq_t g1 = gcd(res, (seq_t)denom);
        res = res / g1;
        denom = denom / (idx_t)g1;

        /* then cancel common factors between mult and the remaining denominator */
        seq_t g2 = gcd(mult, (seq_t)denom);
        mult = mult / g2;
        denom = denom / (idx_t)g2;

        /* denom should be 1, but keep the division for safety */
        res = res * mult;
        if (denom != 1)
            res = res / (seq_t)denom;
    }
    return res;
}
