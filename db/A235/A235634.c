/*
 * A235634  Primes whose base-4 representation is also the base-7
 *          representation of a prime.
 * https://oeis.org/A235634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 740, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9769 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235634: primes p such that the base‑4 representation of p, when
 * read as a base‑7 number, is also prime.  Computed by incremental search. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
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

/* Convert a number from base‑4 digits to the number obtained by writing
   the same digits in base‑7. */
static seq_t base4_to_base7(seq_t n) {
    seq_t result = 0;
    seq_t power  = 1;
    while (n > 0) {
        unsigned digit = (unsigned)(n % 4);
        result += digit * power;
        power  *= 7;
        n      /= 4;
    }
    return result;
}

/* Cache for already found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_candidate = 3;        /* next odd number to test after 2 */

/* Append a term to the cache, expanding storage if necessary. */
static void append_cache(seq_t val) {
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) exit(EXIT_FAILURE);   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;       /* undefined for offset 1 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t candidate;

        if (cache_len == 0) {   /* handle the prime 2 specially */
            candidate = 2;
            if (isPrimeGeneric(candidate)) {
                seq_t q = base4_to_base7(candidate);
                if (isPrimeGeneric(q))
                    append_cache(candidate);
            }
            next_candidate = 3;
            continue;
        }

        candidate = next_candidate;

        if (isPrimeGeneric(candidate)) {
            seq_t q = base4_to_base7(candidate);
            if (isPrimeGeneric(q))
                append_cache(candidate);
        }

        /* Advance to the next odd number. */
        next_candidate += 2;
    }

    return cache[n - 1];
}
