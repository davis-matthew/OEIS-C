/*
 * A100494  Primes of the form 47*k + 3.
 * https://oeis.org/A100494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 143, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 997 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime of the form 47*k + 3 by brute‑force search. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 1, but harmless */

    idx_t found = 0;
    uint64_t counter = 0;
    const uint64_t MOD = 47ULL;

    while (found < n) {
        uint64_t candidate = MOD * counter + 3ULL;   /* candidate = 47*k + 3 */
        if (is_prime_u64(candidate)) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++counter;
    }

    /* This point is never reached for valid n. */
    return 0;
}
