/*
 * A215384  Primes congruent to {1, 2, 3, 6} mod 17.
 * https://oeis.org/A215384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 972 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime congruent to 1, 2, 3, or 6 (mod 17) via incremental search */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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

seq_t a(idx_t n) {
    static seq_t *primes = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static seq_t next_candidate = 2;

    if (!primes) {
        cache_cap = 64;
        primes = mem_new(seq_t, cache_cap);
        if (!primes) abort();
    }

    while (cache_size < n) {
        seq_t p = next_candidate;

        /* Advance the candidate for the next iteration */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;        /* skip even numbers after 2 */

        /* Prüfen, ob p Primzahl ist und die gewünschte Residenz besitzt */
        if (p == 2 || p % 2 == 1) {          /* p is 2 or an odd number */
            if (isPrimeGeneric(p)) {
                seq_t r = p % 17;
                if (r == 1 || r == 2 || r == 3 || r == 6) {
                    if (cache_size == cache_cap) {
                        cache_cap = cache_cap ? cache_cap << 1 : 64;
                        seq_t *tmp = mem_resize(seq_t, primes, cache_cap);
                        if (!tmp) abort();
                        primes = tmp;
                    }
                    primes[cache_size++] = p;
                }
            }
        }
    }

    return primes[n - 1];
}
