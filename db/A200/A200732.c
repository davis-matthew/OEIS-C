/*
 * A200732  Primes of the form 4*n^3-5.
 * https://oeis.org/A200732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*k^3-5 where k is the n-th positive integer such that
 *            4*k^3-5 is prime
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 157, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 46 terms through n = 1000 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for offset 0, but guard for safety */

    idx_t count = 0;
    idx_t k = 1;

    for (;; ++k) {
        /* Compute 4*k^3 - 5 in 128‑bit to avoid overflow */
        __int128 cube  = (__int128)k * k * k;
        __int128 val128 = cube * 4 - 5;
        uint64_t val = (uint64_t)val128;   /* fits for n up to ~10000 */

        if (is_prime_u64(val)) {
            ++count;
            if (count == n)
                return (seq_t)val;
        }
    }
}
