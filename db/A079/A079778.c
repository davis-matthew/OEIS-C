/*
 * A079778  a(n) = smallest prime number such that a(n) - n is composite.
 * https://oeis.org/A079778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest prime p such that p - n is composite (n ≥ 1). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    /* Convert index to 64‑bit integer for arithmetic; the routine only
       handles indices up to 2⁶⁴‑1 which suffices for all relevant test
       cases. */
    uint64_t N = (uint64_t)n;

    /* Search for the first prime p > n with composite difference p-n. */
    uint64_t p = N + 1;
    if (p < 2) p = 2;                     /* start at the first prime */

    for (;;)
    {
        /* Skip non‑primes. */
        if (!is_prime_u64(p)) {
            ++p;
            continue;
        }

        /* Difference between the prime and the index. */
        uint64_t d = p - N;

        /* d must be > 1 and composite. */
        if (d > 1 && !is_prime_u64(d))
            return (seq_t)p;

        ++p;
    }
}
