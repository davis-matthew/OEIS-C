/*
 * A051709  a(n) = sigma(n) + phi(n) - 2n.
 * https://oeis.org/A051709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) + phi(n) - 2*n
 * Range:     u8 n <= 239, u16 n <= 30239, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 95 terms through n = 9206 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/totient.h"

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
    if (n == 0) return 0;                /* Undefined for n = 0, return 0 */

    seq_t sigma_val = sigma_of(n);       /* Sum of divisors of n */
    seq_t phi_val   = totient(n);            /* Euler totient of n */

    seq_t result = sigma_val;
    result += phi_val;                   /* result = sigma + phi */
    result -= (seq_t)n * (seq_t)2;       /* subtract 2*n */

    return result;
}
