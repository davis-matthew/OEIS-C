/*
 * A070935  Largest proper divisor of n^2.
 * https://oeis.org/A070935
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest proper divisor of n² = n² / spf(n) = n*A032742(n)
 * Range:     u8 n <= 23, u16 n <= 363, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9789 (2026-11-01)
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 2; values below that are irrelevant. */
    if (n < 2) return 0ULL;          /* arbitrary */
    uint64_t spf = smallest_prime_factor((uint64_t)n);
    seq_t nn = (seq_t)n;
    /* n² / spf(n) gives the largest proper divisor of n². */
    return nn * nn / (seq_t)spf;
}
