/*
 * A227733  a(n) = sigma(2*n)^2 - sigma(n)^2.
 * https://oeis.org/A227733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(2*n)^2 - sigma(n)^2
 * Range:     u8 n <= 4, u16 n <= 59, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 979 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* compute the sum of divisors of n */
static seq_t sigma_single(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            sum += (seq_t)i;
            idx_t j = n / i;
            if (j != i)
                sum += (seq_t)j;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t s1 = sigma_single(2 * n);
    seq_t s2 = sigma_single(n);
    return s1 * s1 - s2 * s2;
}
