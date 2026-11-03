/*
 * A233541  a(n) = sigma(n) + phi(n) + d(n).
 * https://oeis.org/A233541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000203(n) + A000010(n) + A000005(n)
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9198 (2026-11-01)
 * Helpers:   num_divisors, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = sigma(n) + phi(n) + d(n) */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"
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

seq_t a(idx_t n) {
    seq_t sum = sigma_of(n);          /* sum of divisors (sigma) */
    sum += totient(n);                    /* Euler's totient */
    sum += (seq_t)num_divisors(n); /* number of divisors (d) */
    return sum;
}
