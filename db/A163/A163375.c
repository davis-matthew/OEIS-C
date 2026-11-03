/*
 * A163375  a(n) = sigma(tau(phi(n))).
 * https://oeis.org/A163375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(tau(phi(n)))
 * Range:     u8 n <= 332, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 88 terms through n = 969 (2026-11-01)
 * Helpers:   num_divisors, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the n-th term of OEIS sequence A163375.
 * a(n) = sigma(tau(phi(n))) where
 *   phi(n)   = Euler's totient function,
 *   tau(m)   = number of divisors of m,
 *   sigma(t) = sum of divisors of t. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/num_divisors.h"
#include "../utils/totient_seq.h"

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
    /* Compute φ(n) */
    seq_t phi = totient_seq((seq_t)n);

    /* divisor_count_u64 expects a uint64_t argument */
    idx_t phi_u = (idx_t)phi;
    idx_t d = num_divisors(phi_u);

    /* Return σ(d) */
    return sigma_of(d);
}
