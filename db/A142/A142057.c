/*
 * A142057  Primes congruent to 14 mod 33.
 * https://oeis.org/A142057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 323, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 994 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  primes congruent to 14 modulo 33
   a(n) = the n‑th prime p with p ≡ 14 (mod 33),  a(1) = 47
   The function builds the primes on demand, caching the results.
*/
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Prime test for seq_t values */
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

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

/* Return the n‑th term of A142057 for any seq_t type */
seq_t a(idx_t n)
{
    /* Undefined for n < 1 – return 0 as a harmless sentinel */
    if (n == 0)
        return 0;

    /* Static cache of primes found so far */
    static seq_t *cache = NULL;
    static size_t  len   = 0;
    static size_t  cap   = 0;

    /* The next candidate to test, starting at the first number ≡14 (mod 33) */
    static seq_t next = 14;

    /* Keep generating candidates until we have the requested index */
    while (len < n) {
        if (isPrimeGeneric(next)) {
            /* Expand cache if necessary */
            if (len >= cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)   /* allocation failure – give up */
                    return 0;
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = next;
        }
        /* Move to the next number ≡14 (mod 33) */
        next += 33;
    }

    return cache[n - 1];
}
