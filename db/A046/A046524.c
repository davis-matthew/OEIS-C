/*
 * A046524  Number of coverings of Klein bottle with n lists.
 * https://oeis.org/A046524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = d(n) for odd n, a(n) = [3*d(n)+sigma(n/2)-d(n/2)]/2 for
 *            even n
 * Range:     u8 n <= 335, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 103 terms through n = 985 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
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

seq_t a(idx_t n) {
    if (n & 1ULL) { /* odd n */
        return (seq_t)num_divisors(n);
    } else { /* even n */
        const seq_t d_n  = (seq_t)num_divisors(n);
        const idx_t  n2  = n >> 1;
        const seq_t d_n2 = (seq_t)num_divisors(n2);
        const seq_t sigma_n2 = sigma_of(n2);
        const seq_t numerator = ((seq_t)3 * d_n) + sigma_n2 - d_n2;
        return numerator / 2;
    }
}
