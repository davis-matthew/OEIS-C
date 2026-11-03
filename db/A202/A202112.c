/*
 * A202112  Numbers k such that 90*k + 79 is prime.
 * https://oeis.org/A202112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   numbers k such that 90*k + 79 is prime. a(n) returns the nth
 *            such k
 * Range:     u8 n <= 1, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    /* Count how many k have met the condition. */
    idx_t count = 0;

    for (idx_t k = 0; ; ++k)
    {
        /* Compute the candidate number 90*k + 79 entirely in seq_t to avoid overflow. */
        seq_t cand = (seq_t)90 * (seq_t)k + (seq_t)79;

        if (isPrimeGeneric(cand))
        {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
