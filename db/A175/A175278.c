/*
 * A175278  Base-6 pandigital primes: primes having at least one of each digit
 *          0,1,2,3,4,5 when written in base 6.
 * https://oeis.org/A175278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 41, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 49 terms through n = 9332 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base‑6 pandigital primes – brute‑force search:
   iterate integers >= 6^6; keep those containing all digits 0..5
   in base‑6 representation and are prime; return nth such prime. */
/* This implementation is memoised; results are cached so
   subsequent calls are fast. */

#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1 */

    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t next_candidate = 46656; /* 6^6 */

    /* generate primes until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t cand = next_candidate++;
        /* check that cand contains all digits 0..5 in base‑6 */
        unsigned mask = 0;
        seq_t tmp = cand;
        while (tmp > 0) {
            unsigned d = (unsigned)(tmp % (seq_t)6);
            mask |= 1u << d;
            tmp /= (seq_t)6;
        }
        if (mask != 0x3Fu) continue;   /* not pandigital */

        /* primality test */
        if (isPrimeGeneric(cand)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0 ? 8 : cache_cap * 2);
                seq_t *p = mem_resize(seq_t, cache, new_cap);
                if (!p) return 0;          /* allocation failure */
                cache = p;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
