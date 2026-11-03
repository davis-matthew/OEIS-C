/*
 * A062069  a(n) = sigma(d(n)), where d(k) is the number of divisors function
 *          (A000005) and sigma(k) is the sum of divisors function (A000203).
 * https://oeis.org/A062069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(d(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 105 terms through n = 9926 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the helper functions from the utils library */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"

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
    /* n is guaranteed to be >= 1 by the OEIS offset. */
    /* Compute d(n) – the number of divisors of n. */
    seq_t dcnt = (seq_t)num_divisors((uint64_t)n);

    /* Compute sigma(d(n)) – the sum of divisors of d(n). */
    return sigma_of((idx_t)dcnt);
}
