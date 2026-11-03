/*
 * A218127  Number of 10-ary sequences with primitive period n.
 * https://oeis.org/A218127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 10^d * mu(n/d) for n>0, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 977 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;

    /* iterate over divisors of n */
    for (idx_t d = (idx_t)1; d * d <= n; ++d)
    {
        if (n % d == (idx_t)0)
        {
            /* first divisor d */
            idx_t div1 = d;
            int mu1 = mu_of(n / div1);
            if (mu1 != 0)
            {
                seq_t term = pow10(div1);
                if (mu1 > 0)
                    sum += term;
                else            /* mu1 == -1 */
                    sum -= term;
            }

            /* paired divisor n/d */
            idx_t div2 = n / d;
            if (div2 != div1)
            {
                int mu2 = mu_of(n / div2);
                if (mu2 != 0)
                {
                    seq_t term = pow10(div2);
                    if (mu2 > 0)
                        sum += term;
                    else            /* mu2 == -1 */
                        sum -= term;
                }
            }
        }
    }

    return sum;
}
