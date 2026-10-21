/*
 * A217668  G.f.: Sum_{n>=0} x^n*(1 + x^n)^n.
 * https://oeis.org/A217668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} binomial(n/d, d-1) for n>0, a(0)=1
 * Range:     u8 n <= 39, u16 n <= 139, u32 n <= 441, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            sum += binom(n / d1, d1 - 1);
            if (d1 != d2) {
                sum += binom(n / d2, d2 - 1);
            }
        }
    }
    return sum;
}
