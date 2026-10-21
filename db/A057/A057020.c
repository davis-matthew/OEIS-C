/*
 * A057020  Numerator of (sum of divisors of n / number of divisors of n).
 * https://oeis.org/A057020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of (σ(n)/τ(n)) where σ(n) is the sum of
 *            divisors of n and τ(n) the number of divisors of n. The
 *            numerator is obtained by reducing the fraction σ(n)/τ(n) to
 *            lowest terms
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/gcd.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset starts at 1; return 0 for safety. */
        return 0;

    seq_t sigma = sigma_of(n);                      /* σ(n) */
    seq_t tau   = (seq_t)divisor_count_u64((uint64_t)n); /* τ(n) */

    seq_t g = gcd(sigma, tau);                      /* common divisor */
    return sigma / g;                               /* numerator of reduced fraction */
}
