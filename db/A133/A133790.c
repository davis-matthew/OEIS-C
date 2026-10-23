/*
 * A133790  A014963*A100994.
 * https://oeis.org/A133790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * p if n is a (non‑trivial) power of a prime p,
 *            otherwise a(n) = 1. (a(1) = 1). This is equivalent to the
 *            product of A014963(n) and A100994(n)
 * Range:     u8 n <= 16, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   is_prime_power_and_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_power_and_factor.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t p;
    if (!is_prime_power_and_factor(n, &p))
        return (seq_t)1;

    /* n and p are each at most 64‑bit; cast to seq_t for multiplication. */
    return (seq_t)n * (seq_t)p;
}
