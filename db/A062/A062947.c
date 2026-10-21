/*
 * A062947  a(n) = binomial(n,floor(n/7)).
 * https://oeis.org/A062947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n, floor(n/7))
 * Range:     u8 n <= 20, u16 n <= 34, u32 n <= 62, u64 n <= 116,
 *            u128 n <= 223, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Compute binomial(n, k) with cancelation to avoid intermediate overflow.
   It reduces the denominator with the current result and then with the
   numerator before each multiplication. */
static seq_t binom_reduce(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;                /* use symmetry */

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;

        /* cancel common factors between res and den */
        seq_t g = gcd(res, den);
        if (g > 1) {
            res /= g;
            den /= g;
        }

        /* cancel common factors between num and den */
        g = gcd(num, den);
        if (g > 1) {
            num /= g;
            den /= g;
        }

        /* after reductions den should be 1 */
        res *= num;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t k = n / 7;          /* floor(n/7) for unsigned arithmetic */
    return binom_reduce(n, k);
}
