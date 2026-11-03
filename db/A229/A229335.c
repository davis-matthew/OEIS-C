/*
 * A229335  Sum of sums of elements of subsets of divisors of n.
 * https://oeis.org/A229335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) * 2^(tau(n)-1) (OEIS A229335)
 * Range:     u8 n <= 11, u16 n <= 59, u32 n <= 359, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 988 (2026-11-01)
 * Helpers:   num_divisors, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                /* size_t */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/num_divisors.h"
#include "../utils/pow2.h"

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
    /* sigma(n): sum of divisors */
    seq_t sigma = sigma_of(n);

    /* tau(n): number of divisors */
    idx_t tau = (idx_t)num_divisors(n);

    /* 2^(tau-1) */
    seq_t pow_val = pow2(tau - 1);

    /* product */
    return sigma * pow_val;
}
