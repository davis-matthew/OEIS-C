/*
 * A201605  Primes of the form 7n^2 + 4.
 * https://oeis.org/A201605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 2832, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A201605: primes of the form 7*k^2 + 4. This implementation
   searches for the nth prime of that form. The search skips even values
   of k, since 7*(2m)^2 + 4 is always divisible by 4.                  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_uint64.h"

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;  /* undefined for n < 1, return 0 as placeholder */
    uint64_t count = 0;
    uint64_t k = 1;              /* start from k=1, increment by 2 (odd k only) */
    while (true) {
        /* Compute p = 7*k^2 + 4, using 128‑bit intermediate to avoid overflow. */
        __uint128_t ksq = (__uint128_t)k * k;
        uint64_t p = 7ULL * (uint64_t)ksq + 4ULL;
        if (is_prime_uint64(p)) {
            ++count;
            if (count == n) {
                return (seq_t)p;
            }
        }
        k += 2;    /* skip even k */
    }
}
