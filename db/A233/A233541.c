/*
 * A233541  a(n) = sigma(n) + phi(n) + d(n).
 * https://oeis.org/A233541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000203(n) + A000010(n) + A000005(n)
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = sigma(n) + phi(n) + d(n) */

#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/phi.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n) {
    seq_t sum = sigma_of(n);          /* sum of divisors (sigma) */
    sum += phi(n);                    /* Euler's totient */
    sum += (seq_t)divisor_count_u64(n); /* number of divisors (d) */
    return sum;
}
