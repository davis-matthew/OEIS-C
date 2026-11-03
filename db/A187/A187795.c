/*
 * A187795  Sum of the abundant divisors of n.
 * https://oeis.org/A187795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, sigma(d) > 2*d} d * (Sum of the abundant
 *            divisors of n)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 112 terms through n = 9559 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* iterate over divisors up to sqrt(n) */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i != 0)
            continue;

        seq_t d1 = (seq_t)i;
        if (sigma_of((idx_t)d1) > d1 + d1)
            sum += d1;

        idx_t j = n / i;
        if (j == i)                /* avoid double counting perfect square divisors */
            continue;

        seq_t d2 = (seq_t)j;
        if (sigma_of((idx_t)d2) > d2 + d2)
            sum += d2;
    }
    return sum;
}
