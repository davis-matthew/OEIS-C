/*
 * A228411  G.f.: ( (1 - sqrt(1-32*x)) / (16*x) )^(1/4).
 * https://oeis.org/A228411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8^n*binomial(2*n + 1/4, n)/(8*n + 1) Recurrence:
 *            n*(4*n+1)*a(n) = 2*(8*n-3)*(8*n-7)*a(n-1), a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        seq_t num = (seq_t)2 * (seq_t)(8*i - 3) * (seq_t)(8*i - 7);
        seq_t den = (seq_t)i * (seq_t)(4*i + 1);

        seq_t g = gcd(res, den);
        res = res / g;
        den = den / g;

        g = gcd(num, den);
        num = num / g;
        den = den / g;

        /* Now den should be 1; multiply by the reduced numerator */
        res = res * num;
        if (den != (seq_t)1)
            res = res / den;   /* safety: should not happen */
    }
    return res;
}
