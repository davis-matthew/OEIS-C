/*
 * A103390  Natural numbers but with nonprimes squared.
 * https://oeis.org/A103390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n - 1) when (n - 1) is prime; otherwise a(n) = (n - 1)^2
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9805 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    idx_t m = n - 1;          /* integer whose square or value is returned */
    if (is_prime_u64(m))
        return (seq_t)m;      /* keep primes unchanged */
    seq_t mm = (seq_t)m;
    return mm * mm;          /* square non‑primes (including 0 and 1) */
}
