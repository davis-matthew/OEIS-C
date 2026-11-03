/*
 * A165797  a(n) = n^( sigma(n) - tau(n) ).
 * https://oeis.org/A165797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(sigma(n) - tau(n)) (sigma = sum of divisors, tau =
 *            number of divisors)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 49 (2026-11-01)
 * Helpers:   ipow_idx, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
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
    if (n == 0) return (seq_t)1;          /* offset is 1, 0 is unused */
    seq_t sigma = sigma_of(n);            /* sigma(n) */
    uint64_t tau  = num_divisors(n); /* tau(n) */
    idx_t exp = (idx_t)(sigma - tau);     /* exponent as idx_t */
    return ipow_idx((seq_t)n, exp);        /* n^exp */
}
