/*
 * A105052  Write a(n) as a four-bit number; those bits state whether 10n+1,
 *          10n+3, 10n+7 and 10n+9 are primes.
 * https://oeis.org/A105052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Encoding prime‑ness of the four twiddle numbers 10n+r
   with r = 1,3,7,9.  Bits are assigned MSB → LSB : 1,3,7,9. */
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(uint64_t) */

seq_t a(idx_t n)
{
    /* Use 64‑bit arithmetic for the base; the result cannot overflow
       for any n that is reasonably testable. */
    uint64_t base = 10ULL * (uint64_t)n;
    seq_t val = 0U;

    /* set bit 3 if 10n+1 is prime */
    if (is_prime(base + 1ULL))
        val |= (seq_t)1U << 3;

    /* set bit 2 if 10n+3 is prime */
    if (is_prime(base + 3ULL))
        val |= (seq_t)1U << 2;

    /* set bit 1 if 10n+7 is prime */
    if (is_prime(base + 7ULL))
        val |= (seq_t)1U << 1;

    /* set bit 0 if 10n+9 is prime */
    if (is_prime(base + 9ULL))
        val |= (seq_t)1U << 0;

    return val;
}
