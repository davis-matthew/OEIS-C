/*
 * A053181  Composite numbers ending in 9.
 * https://oeis.org/A053181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the n‑th composite number of the form 10*k + 9 (k ≥ 0)
 * Range:     u8 n <= 13, u16 n <= 4931, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9239 (2026-11-03)
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

/* External integer types are provided by the build */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for offset 0 */

    idx_t count = 0;
    idx_t k = 0;
    seq_t val;

    for (;; ++k) {
        val = ((seq_t)10 * k) + (seq_t)9;  /* candidate 10k+9 */
        if (!isPrimeGeneric(val)) {        /* composite check */
            ++count;
            if (count == n)
                return val;
        }
    }
}
