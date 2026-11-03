/*
 * A058063  Number of prime factors (when counted with multiplicity) of
 *          sigma(n), the sum of divisors of n.
 * https://oeis.org/A058063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001222(A000203(n)) (Number of prime factors of sigma(n),
 *            multiplicity)
 * Range:     u8 n <= 389, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9846 (2026-11-01)
 * Helpers:   bigomega_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/bigomega_seq.h"

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
    /* The sequence is defined for n >= 1.  For safety we
       return 0 when n == 0 (does not affect any OEIS test). */
    if (n == 0)
        return (seq_t)0;

    seq_t sigma = sigma_of(n);
    return bigomega_seq(sigma);
}
