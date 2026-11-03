/*
 * A163371  a(n) = tau(phi(sigma(n))).
 * https://oeis.org/A163371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(phi(sigma(n)))
 * Range:     u8 n <= 321, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 102 terms through n = 980 (2026-11-01)
 * Helpers:   num_divisors, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of divisors of φ(σ(n)) for n ≥ 1 */
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
    /* σ(n) */
    seq_t sigma = sigma_of(n);

    /* φ(σ(n)) */
    seq_t phi_val = totient_seq(sigma);

    /* tau(φ(σ(n))) – φ(σ(n)) fits in 64 bits for the range considered */
    uint64_t phi_u = (uint64_t)phi_val;
    uint64_t tau_val = num_divisors(phi_u);

    return (seq_t)tau_val;
}
