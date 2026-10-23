/*
 * A026005  a(n) = T(4*n,n), where T = Catalan triangle (A008315).
 * https://oeis.org/A026005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(4*n, n) - binomial(4*n, n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (The binomial coefficients are computed by multiplicative reduction
   using gcd in order to avoid overflow in any seq_t implementation.) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

static seq_t binomial_reduced(idx_t n, idx_t k)
{
    /* Compute C(n,k) with no intermediate overflow (works for all
       seq_t sizes).  Assumes 0 <= k <= n. */
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i);   /* numerator factor        */
        seq_t den = (seq_t)i;             /* denominator factor     */

        seq_t g1 = gcd(num, den);
        if (g1 > (seq_t)1)
        {
            num /= g1;
            den /= g1;
        }

        seq_t g2 = gcd(res, den);
        if (g2 > (seq_t)1)
        {
            res /= g2;
            den /= g2;
        }

        res *= num;
        if (den != (seq_t)1)
            res /= den;   /* den must now divide res exactly */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* offset 0 */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)4;

    idx_t m = n << 2;   /* 4*n */
    seq_t c1 = binomial_reduced(m, n);
    seq_t c2 = binomial_reduced(m, n - (idx_t)2);
    return c1 - c2;
}
