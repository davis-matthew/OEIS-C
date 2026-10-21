/*
 * A058622  a(n) = 2^(n-1) - ((1+(-1)^n)/4)*binomial(n, n/2).
 * https://oeis.org/A058622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) - ((1+(-1)^n)/4)*binomial(n, n/2) with a(0) = 0.
 *            Equivalent to a(n) = Σ_{i=0..floor((n-1)/2)} binomial(n,i)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute 2^(n-1) using pow2 from utils, and binomial multiplicatively
   with gcd–cancellation to avoid overflow. */

#include "../utils/pow2.h"
#include "../utils/gcd.h"

static seq_t binom_exact(idx_t n, idx_t k)
{
    /* Reduce k to n-k if that is smaller. */
    if (k > n - k)
        k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = n - k + i;   /* numerator factor */
        seq_t den = i;           /* denominator factor */

        /* cancel common factors between num and den */
        seq_t g = gcd(num, den);
        if (g > 1) {
            num /= g;
            den /= g;
        }

        /* cancel common factors between res and remaining den */
        g = gcd(res, den);
        if (g > 1) {
            res /= g;
            den /= g;
        }

        /* at this stage den should be 1.  If not, divide res by it. */
        if (den > 1)
            res /= den;

        /* multiply res by the remaining numerator factor */
        res *= num;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Odd indices: a(2m+1) = 2^(2m) = 2^(n-1) */
    if (n & 1)
        return pow2(n - 1);

    /* Even indices: a(2m) = 2^(2m-1) - binomial(2m, m)/2 */
    idx_t m = n >> 1;
    seq_t half_power = pow2(n - 1);           /* 2^(n-1) = 2^(2m-1)   */
    seq_t binom_val  = binom_exact(n, m);     /* exact binomial value */
    seq_t half_central = binom_val >> 1;      /* divide by 2 safely */

    return half_power - half_central;
}
